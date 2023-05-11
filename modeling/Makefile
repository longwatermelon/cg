CXX=g++
CXXFLAGS=-std=c++17 -O2 -ggdb -Wall
INC=
LDFLAGS=-L../lib/ -lcurves -lSDL2

.PHONY: libcurve

all: splines

%: demo_%.cpp
	$(CXX) $(CXXFLAGS) $(INC) $^ $(LDFLAGS)

libcurve:
	$(CXX) $(CXXFLAGS) $(INC) -c curves.cpp
	$(AR) $(ARFLAGS) ../lib/libcurves.a curves.o
