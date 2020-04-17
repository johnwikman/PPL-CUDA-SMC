# Makefile for building various Airplane test cases

.PHONY: all airplanes benchmark clean

PARTICLESIZES=200 2000 10000 100000 500000 1000000 10000000

SRCDIR=PPL-SMC/Src
NVCC=nvcc
NVCCFLAGS=-arch=sm_61 -rdc=true -lcudadevrt -std=c++11 -O3 -D GPU

all: airplanes

airplanes:
	make $(foreach size, $(PARTICLESIZES), $(size).airplane)
%.airplane: FORCE
	$(NVCC) $(NVCCFLAGS) -D_PARTICLES_=$* $(SRCDIR)/Airplane/*.cu $(SRCDIR)/Utils/*.cpp -o airplane$*.out
FORCE:

benchmark:
	mkdir -p results
	make $(foreach p, $(shell ls *.out), $(shell basename $(p) .out).benchmark)
%.benchmark:
	nvprof ./$*.out > results/$*.toml 2> results/$*.nvprof

clean:
	rm -f airplane*.out
