CXX=g++
CXXFLAGS=-std=c++17 -ggdb -Wall -Ofast
INC=
LDFLAGS=-L../lib/ -lraytrace -lassimp

LIBSRC=$(wildcard *.cpp)
LIBOBJ=$(LIBSRC:.cpp=.o)

.PHONY: lib

all: main

%: demo/%.cpp
	$(CXX) $(CXXFLAGS) $(INC) $^ $(LDFLAGS)

lib:
	$(CXX) $(CXXFLAGS) $(INC) -c $(LIBSRC)
	$(AR) $(ARFLAGS) ../lib/libraytrace.a $(LIBOBJ)
