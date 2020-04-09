#ifndef AIRPLANE_INCLUDED
#define AIRPLANE_INCLUDED

#include "../Smc/smc.cuh"

struct progState_t {
    floating_t x;
    //int t;
};

typedef progState_t stateType;

const int OBSERVATION_STD = 1;
const int TRANSITION_STD = 1;
const int VELOCITY = 2;
const int MAP_SIZE = 201;
const int ALTITUDE = 70;
const int TIME_STEPS = 100;

const floating_t STARTING_POINT = 20.0;
const floating_t SQRT_TWO_PI = 2.506628274631000502415765284811045253006986740609938316629;
const floating_t SQRT_TWO_PI_OBS_STD = SQRT_TWO_PI * OBSERVATION_STD;
const floating_t TWO_OBS_STD_SQUARED = OBSERVATION_STD * OBSERVATION_STD * 2;


#endif