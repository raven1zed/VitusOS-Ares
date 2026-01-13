#!/bin/bash
set -e

# Define project root
PROJECT_ROOT="/mnt/c/Users/hp/Documents/VitusOS-Ares"
BUILD_DIR="$PROJECT_ROOT/opensef/build"

echo "====== STARTING CLEAN BUILD ======"

# Ensure build directory exists
if [ -d "$BUILD_DIR" ]; then
    echo "Cleaning existing build directory..."
    rm -rf "$BUILD_DIR/*"
else
    mkdir -p "$BUILD_DIR"
fi

cd "$BUILD_DIR"

echo "Configuring with CMake..."
cmake "$PROJECT_ROOT/opensef" -G "Unix Makefiles"

echo "Building project (Parallel)..."
make -j4

echo "====== BUILD COMPLETE ======"
