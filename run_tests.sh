#!/bin/bash

# Script to build and run raytracer tests

set -e

echo "================================"
echo "Raytracer Test Suite Runner"
echo "================================"
echo ""

# Build the project
echo "Step 1: Building project..."
echo "----------------------------"
mkdir -p build
cd build

# Clean build if requested
if [ "$1" == "--clean" ]; then
    echo "Performing clean build..."
    rm -rf *
fi

# Configure and build
CC=gcc CXX=g++ cmake .. > /dev/null 2>&1
make -j$(nproc) > /dev/null 2>&1

if [ $? -eq 0 ]; then
    echo "Build successful!"
else
    echo "Build failed!"
    exit 1
fi

echo ""
echo "Step 2: Running tests..."
echo "----------------------------"
echo ""

# Run tests
if [ "$1" == "--verbose" ] || [ "$2" == "--verbose" ]; then
    ctest --verbose
else
    ctest
fi

TEST_RESULT=$?

echo ""
echo "================================"
echo "Test Summary"
echo "================================"
echo ""

if [ $TEST_RESULT -eq 0 ]; then
    echo "All tests passed!"
else
    echo "Some tests failed (as expected for demonstration)."
    echo ""
    echo "Expected results:"
    echo "  - empty_scene: PASS (edge case)"
    echo "  - single_sphere: PASS (regular case)"
    echo "  - invalid_scene: FAIL (demonstration)"
fi

echo ""
echo "To see detailed output, run:"
echo "  ./run_tests.sh --verbose"
echo ""
echo "To run a specific test:"
echo "  cd build && ctest -R test_name"
echo ""
