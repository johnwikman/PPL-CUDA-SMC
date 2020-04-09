# Makefile for building various Airplane test cases

.PHONY: all airplanes clean

PARTICLESIZES=200 10000 10000000

SRCDIR=PPL-SMC/Src
NVCC=nvcc
NVCCFLAGS=-arch=sm_61 -rdc=true -lcudadevrt -std=c++11 -O3 -D GPU

all: airplanes

airplanes:
	make $(foreach size, $(PARTICLESIZES), $(size).airplane)

%.airplane: FORCE
	$(NVCC) $(NVCCFLAGS) -D_PARTICLES_=$* $(SRCDIR)/Airplane/*.cu $(SRCDIR)/Utils/*.cpp -o airplane$*.out
FORCE:

clean:
	rm -f airplane*.out
