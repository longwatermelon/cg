CXX=g++
CXXFLAGS=-std=c++17 -ggdb -Wall
INC=
LDFLAGS=-L../lib/ -lraytrace

LIBSRC=raytrace.cpp geometry.cpp lighting.cpp util.cpp scene.cpp
LIBOBJ=$(LIBSRC:.cpp=.o)

.PHONY: lib

all: test

%: demo/%.cpp
	$(CXX) $(CXXFLAGS) $(INC) $^ $(LDFLAGS)

lib:
	$(CXX) $(CXXFLAGS) $(INC) -c $(LIBSRC)
	$(AR) $(ARFLAGS) ../lib/libraytrace.a $(LIBOBJ)
