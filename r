#!/bin/bash

set -e  # Exit immediately if a command exits with a non-zero status.

# Create build directory if it doesn't exist
mkdir -p build

# Navigate to build directory
cd build

# Run CMake
cmake ..

# Build the project
make

# Run the application
./ray_tracer

# Navigate back to the root directory
cd ..

echo "Ray tracer execution completed."
