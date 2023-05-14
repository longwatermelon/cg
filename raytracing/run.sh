#!/bin/sh
make lib
make
./a.out
feh out.ppm
