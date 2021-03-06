#include <stdio.h>
#include <math.h>
#include <random>
#include <time.h>

#include "../Smc/smc.cuh"
#include "../Smc/smcImpl.cuh"
#include "airplane.cuh"
#include "airplaneUtils.cuh"
#include "../Utils/distributions.cuh"
#include "../Utils/array.cuh"
#include "../Utils/misc.cuh"

// nvcc -arch=sm_61 -rdc=true Src/Airplane/*.cu Src/Utils/*.cpp -o smc.exe -lcudadevrt -std=c++11 -O3 -D GPU

using namespace std;


floating_t planeX[TIME_STEPS];

BBLOCK_DATA(planeObs, floating_t, TIME_STEPS)
BBLOCK_DATA(mapApprox, floating_t, MAP_SIZE)


void initAirplane() {

    initMap(mapApprox);

    initObservations(planeX, planeObs, mapApprox);

    // Copy data to device pointers, so that they can be accessed from kernels
    COPY_DATA_GPU(planeObs, floating_t, TIME_STEPS)
    COPY_DATA_GPU(mapApprox, floating_t, MAP_SIZE)
}


BBLOCK(particleInit, progState_t, {

    PSTATE.x = sampleUniform(particles, i, 0, MAP_SIZE);

    PC = 1;
    RESAMPLE = false;
})

BBLOCK(propagateAndWeight, progState_t, {

    // Propagate
    PSTATE.x += sampleNormal(particles, i, VELOCITY, TRANSITION_STD);

    // Weight
    WEIGHT(logNormalPDFObs(DATA_POINTER(planeObs)[t], mapLookupApprox(DATA_POINTER(mapApprox), PSTATE.x)));

    if(t >= TIME_STEPS - 1)
        PC = 2;

    RESAMPLE = true;
})

STATUSFUNC({
    // Checks how many particles are close to actual airplane to check for correctness
    int numParticlesClose = 0;
    floating_t minX = 999999;
    floating_t maxX = -1;
    for (int i = 0; i < NUM_PARTICLES; i++) {
        floating_t particleX = PSTATE.x;
        if(abs(particleX - planeX[t]) < 10)
            numParticlesClose++;
        minX = min(minX, particleX);
        maxX = max(maxX, particleX);
    }

    cout << "TimeStep " << t << ", Num particles close to target: " << 100 * static_cast<floating_t>(numParticlesClose) / NUM_PARTICLES << "%, MinX: " << minX << ", MaxX: " << maxX << endl;
})

int main(int argc, char** argv) {

    initAirplane();

    SMCSTART(progState_t)

    INITBBLOCK(particleInit, progState_t)
    INITBBLOCK(propagateAndWeight, progState_t)

    SMCEND(progState_t)
}
