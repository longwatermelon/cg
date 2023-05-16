#!/bin/sh
make lib
make
time ./a.out
feh out.ppm
