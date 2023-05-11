CXX=g++
CXXFLAGS=-std=c++17 -ggdb -Wall
INC=
LDFLAGS=-L../lib/ -lcurves -lanimation -lSDL2

.PHONY: %

all: surface

%: demo/%.cpp
	$(CXX) $(CXXFLAGS) $(INC) -I. $^ $(LDFLAGS)

%:
	$(CXX) $(CXXFLAGS) $(INC) -c $@.cpp
	$(AR) $(ARFLAGS) ../lib/lib$@.a $@.o
