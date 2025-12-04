# Raytracer End-to-End Test Suite

This directory contains end-to-end tests for the raytracer project using CTest.

## Overview

The test suite validates the raytracer's output by comparing generated images against reference images. This ensures that optimizations and code changes do not introduce regressions.

## Test Framework

- **Framework**: CTest (integrated with CMake)
- **Comparison Method**: Pixel-by-pixel image comparison with tolerance threshold
- **Test Types**: End-to-end tests that validate complete rendering pipeline

## Test Cases

### 1. Empty Scene Test (Edge Case)
- **Scene**: `empty-scene.json`
- **Description**: Tests the raytracer's ability to handle a scene with no objects
- **Purpose**: Validates edge case behavior and ensures the renderer doesn't crash with empty input
- **Expected Result**: PASS - Should produce a solid ambient-colored image

### 2. Single Sphere Test (Regular Case)
- **Scene**: `single-sphere.json`
- **Description**: Tests basic rendering with one sphere, one light source, and reflections
- **Purpose**: Validates normal rendering workflow with typical scene elements
- **Expected Result**: PASS - Should match the reference image exactly

### 3. Invalid Scene Test (Failure Demonstration)
- **Scene**: `invalid-scene.json`
- **Description**: Demonstrates test failure detection
- **Purpose**: Shows that the test suite can reliably detect when rendering output differs from expectations
- **Expected Result**: FAIL - Intentionally uses wrong reference image to demonstrate failure detection

## Running the Tests

### Build the Project
```bash
cd /workspace
mkdir -p build && cd build
CC=gcc CXX=g++ cmake ..
make -j$(nproc)
```

### Run All Tests
```bash
cd build
ctest
```

### Run Tests with Verbose Output
```bash
cd build
ctest --verbose
```

### Run a Specific Test
```bash
cd build
ctest -R empty_scene
```

### Re-run Failed Tests
```bash
cd build
ctest --rerun-failed --output-on-failure
```

## Test Structure

Each test consists of two phases:

1. **Render Phase**: Executes the raytracer with a test scene
   - Test name: `{test_name}_render`
   - Generates output image in build directory

2. **Compare Phase**: Compares output against reference image
   - Test name: `{test_name}_compare`
   - Uses `image_compare` utility
   - Depends on successful completion of render phase

## Image Comparison

The `image_compare` utility compares two PNG images:

- Checks image dimensions
- Performs pixel-by-pixel RGB comparison
- Applies tolerance threshold (1% per pixel, <0.1% total difference)
- Reports statistics on differences
- Returns exit code 0 for match, 1 for mismatch

## Test Characteristics

### Consistency
- Tests use fixed resolution (320x240) for fast execution
- Reference images are version-controlled
- Deterministic rendering ensures repeatable results

### Repeatability
- Tests can be run multiple times with identical results
- No randomization or time-dependent behavior
- Clean state between test runs

### Regression Detection
- Pixel-level accuracy ensures any visual changes are caught
- Tolerance threshold prevents false positives from floating-point variations
- Failed tests clearly indicate which image differs

## Directory Structure

```
tests/
├── CMakeLists.txt           # CTest configuration
├── README.md                # This file
├── image_compare.cpp        # Image comparison utility
├── scenes/                  # Test scene definitions
│   ├── empty-scene.json
│   ├── single-sphere.json
│   └── invalid-scene.json
└── expected/                # Reference images
    ├── empty-scene-expected.png
    ├── single-sphere-expected.png
    └── invalid-scene-expected.png
```

## Adding New Tests

To add a new test:

1. Create a scene JSON file in `tests/scenes/`
2. Generate reference image:
   ```bash
   ./build/raytracer tests/scenes/your-scene.json tests/expected/your-scene-expected.png
   ```
3. Add test in `tests/CMakeLists.txt`:
   ```cmake
   add_raytracer_test(
       your_test_name
       "your-scene.json"
       "your-scene-expected.png"
   )
   ```
4. Rebuild and run tests

## Notes

- Test images use lower resolution (320x240) for faster execution
- All tests have a 60-second timeout to prevent hangs
- The invalid_scene test is expected to fail as a demonstration
