# Test suite for raytracer

# Build the image comparison utility
add_executable(image_compare image_compare.cpp)
target_link_libraries(image_compare PRIVATE lodepng)
target_include_directories(image_compare PRIVATE 
    "${PROJECT_SOURCE_DIR}/src/lodepng"
)

# Helper function to add a raytracer test
function(add_raytracer_test TEST_NAME SCENE_FILE EXPECTED_IMAGE)
    # Generate the output path
    set(OUTPUT_IMAGE "${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}_output.png")
    
    # Add test that runs the raytracer
    add_test(
        NAME ${TEST_NAME}_render
        COMMAND raytracer 
            "${CMAKE_CURRENT_SOURCE_DIR}/scenes/${SCENE_FILE}"
            "${OUTPUT_IMAGE}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    
    # Add test that compares the output with expected image
    add_test(
        NAME ${TEST_NAME}_compare
        COMMAND image_compare 
            "${OUTPUT_IMAGE}"
            "${CMAKE_CURRENT_SOURCE_DIR}/expected/${EXPECTED_IMAGE}"
    )
    
    # Make sure compare test runs after render test
    set_tests_properties(${TEST_NAME}_compare PROPERTIES 
        DEPENDS ${TEST_NAME}_render
    )
endfunction()

# Test 1: Empty scene (edge case)
# This tests that the raytracer can handle a scene with no objects
add_raytracer_test(
    empty_scene
    "empty-scene.json"
    "empty-scene-expected.png"
)

# Test 2: Single sphere (regular case)
# This tests basic rendering with one object, one light, and reflections
add_raytracer_test(
    single_sphere
    "single-sphere.json"
    "single-sphere-expected.png"
)

# Test 3: Invalid scene (should fail - for demonstration)
# This scene will produce different output to demonstrate test failure
add_raytracer_test(
    invalid_scene
    "invalid-scene.json"
    "invalid-scene-expected.png"
)

# Set timeout for all tests (in seconds)
set_tests_properties(
    empty_scene_render
    empty_scene_compare
    single_sphere_render
    single_sphere_compare
    invalid_scene_render
    invalid_scene_compare
    PROPERTIES TIMEOUT 60
)
