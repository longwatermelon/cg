CXX=g++
CXXFLAGS=-std=c++17 -ggdb -Wall -O3
INC=`pkg-config --cflags opencv4`
LDFLAGS=-L../lib/ -lraytrace `pkg-config --libs opencv4`

LIBSRC=$(wildcard *.cpp)
LIBOBJ=$(LIBSRC:.cpp=.o)

.PHONY: lib

all: main

%: demo/%.cpp
	$(CXX) $(CXXFLAGS) $(INC) $^ $(LDFLAGS)

lib:
	$(CXX) $(CXXFLAGS) $(INC) -c $(LIBSRC)
	$(AR) $(ARFLAGS) ../lib/libraytrace.a $(LIBOBJ)
