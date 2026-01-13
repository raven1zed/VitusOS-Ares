#!/bin/bash
set -e
echo "Starting Emergency Rebuild..."
cd opensef/build
cmake .. -G "Unix Makefiles"
make -j4
cd ../..
echo "Build complete. Running tests..."
bash scripts/definitive_test.sh
