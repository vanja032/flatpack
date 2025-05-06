#!/bin/bash

set -e  # Stop on first error

# Create build directory if it does not exist
mkdir -p build
rm -rf build/*
cd build

# Configure project
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build project
cmake --build .