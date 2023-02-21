#!/bin/sh


cd build 
cmake ..
make -j4
./executable
