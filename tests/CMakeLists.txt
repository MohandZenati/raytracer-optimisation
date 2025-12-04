# Tests configuration for raytracer

# Helper function to add a raytracer test
function(add_raytracer_test TEST_NAME SCENE_FILE TOLERANCE)
    set(OUTPUT_IMAGE "${CMAKE_CURRENT_BINARY_DIR}/output_${TEST_NAME}.png")
    set(REFERENCE_IMAGE "${CMAKE_CURRENT_SOURCE_DIR}/references/${TEST_NAME}.png")
    
    # Add test that renders the scene and compares with reference
    add_test(
        NAME ${TEST_NAME}
        COMMAND ${CMAKE_COMMAND}
        -DRAYTRACER=$<TARGET_FILE:raytracer>
        -DIMAGE_COMPARE=$<TARGET_FILE:image_compare>
        -DSCENE_FILE=${SCENE_FILE}
        -DOUTPUT_IMAGE=${OUTPUT_IMAGE}
        -DREFERENCE_IMAGE=${REFERENCE_IMAGE}
        -DTOLERANCE=${TOLERANCE}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/run_test.cmake
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    
    # Set test properties
    set_tests_properties(${TEST_NAME} PROPERTIES
        TIMEOUT 300
        LABELS "e2e"
    )
endfunction()

# Add performance test function
function(add_performance_test TEST_NAME SCENE_FILE)
    add_test(
        NAME ${TEST_NAME}_performance
        COMMAND ${CMAKE_COMMAND}
        -DRAYTRACER=$<TARGET_FILE:raytracer>
        -DSCENE_FILE=${SCENE_FILE}
        -DOUTPUT_IMAGE=${CMAKE_CURRENT_BINARY_DIR}/perf_${TEST_NAME}.png
        -P ${CMAKE_CURRENT_SOURCE_DIR}/run_performance_test.cmake
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    
    set_tests_properties(${TEST_NAME}_performance PROPERTIES
        TIMEOUT 600
        LABELS "performance"
    )
endfunction()

# Define tests
# Simple test - regular use case
add_raytracer_test(
    "simple_test"
    "${CMAKE_CURRENT_SOURCE_DIR}/scenes/simple-test.json"
    "1.0"
)

# Edge case: empty scene
add_raytracer_test(
    "edge_case_empty"
    "${CMAKE_CURRENT_SOURCE_DIR}/scenes/edge-case-empty.json"
    "1.0"
)

# Edge case: single sphere
add_raytracer_test(
    "edge_case_single_sphere"
    "${CMAKE_CURRENT_SOURCE_DIR}/scenes/edge-case-single-sphere.json"
    "1.0"
)

# Complex test - multiple objects with reflections
add_raytracer_test(
    "complex_test"
    "${CMAKE_CURRENT_SOURCE_DIR}/scenes/complex-test.json"
    "1.0"
)

# Performance tests
add_performance_test(
    "simple"
    "${CMAKE_CURRENT_SOURCE_DIR}/scenes/simple-test.json"
)

add_performance_test(
    "complex"
    "${CMAKE_CURRENT_SOURCE_DIR}/scenes/complex-test.json"
)

# Add a custom target to generate reference images
add_custom_target(generate_references
    COMMAND ${CMAKE_COMMAND}
    -DRAYTRACER=$<TARGET_FILE:raytracer>
    -DREFERENCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/references
    -DTEST_SCENES_DIR=${CMAKE_CURRENT_SOURCE_DIR}/scenes
    -P ${CMAKE_CURRENT_SOURCE_DIR}/generate_references.cmake
    DEPENDS raytracer
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating reference images for tests"
)
