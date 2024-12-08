#!/bin/bash

cmake -S ./src/ -B ./build/ -DCMAKE_CXX_FLAGS=-O2
cd build
make