CC=g++
NVCC=nvcc

debug:
	$(NVCC) -G -g -o main main.cu data.cpp strata.cpp rawunits.cpp stratamapping.cpp

all: main.cu
	$(NVCC) -o main main.cu data.cpp strata.cpp rawunits.cpp stratamapping.cpp

