# Script to run a single raytracer test
# This script is called by CTest

# Check that all required variables are set
if(NOT DEFINED RAYTRACER)
    message(FATAL_ERROR "RAYTRACER not defined")
endif()

if(NOT DEFINED IMAGE_COMPARE)
    message(FATAL_ERROR "IMAGE_COMPARE not defined")
endif()

if(NOT DEFINED SCENE_FILE)
    message(FATAL_ERROR "SCENE_FILE not defined")
endif()

if(NOT DEFINED OUTPUT_IMAGE)
    message(FATAL_ERROR "OUTPUT_IMAGE not defined")
endif()

if(NOT DEFINED REFERENCE_IMAGE)
    message(FATAL_ERROR "REFERENCE_IMAGE not defined")
endif()

if(NOT DEFINED TOLERANCE)
    set(TOLERANCE "0.0")
endif()

message("========================================")
message("Running raytracer test")
message("  Scene:     ${SCENE_FILE}")
message("  Output:    ${OUTPUT_IMAGE}")
message("  Reference: ${REFERENCE_IMAGE}")
message("  Tolerance: ${TOLERANCE}")
message("========================================")

# Step 1: Run the raytracer
message("Step 1: Rendering scene...")
execute_process(
    COMMAND ${RAYTRACER} ${SCENE_FILE} ${OUTPUT_IMAGE}
    RESULT_VARIABLE RENDER_RESULT
    OUTPUT_VARIABLE RENDER_OUTPUT
    ERROR_VARIABLE RENDER_ERROR
)

if(NOT RENDER_RESULT EQUAL 0)
    message(FATAL_ERROR "Raytracer failed with exit code ${RENDER_RESULT}\nOutput: ${RENDER_OUTPUT}\nError: ${RENDER_ERROR}")
endif()

message("Rendering completed successfully")
message("${RENDER_OUTPUT}")

# Step 2: Check if output image exists
if(NOT EXISTS ${OUTPUT_IMAGE})
    message(FATAL_ERROR "Output image was not created: ${OUTPUT_IMAGE}")
endif()

# Step 3: Check if reference image exists
if(NOT EXISTS ${REFERENCE_IMAGE})
    message(FATAL_ERROR "Reference image not found: ${REFERENCE_IMAGE}")
    message("You may need to generate reference images first using: make generate_references")
endif()

# Step 4: Compare images
message("Step 2: Comparing images...")
execute_process(
    COMMAND ${IMAGE_COMPARE} ${REFERENCE_IMAGE} ${OUTPUT_IMAGE} ${TOLERANCE}
    RESULT_VARIABLE COMPARE_RESULT
    OUTPUT_VARIABLE COMPARE_OUTPUT
    ERROR_VARIABLE COMPARE_ERROR
)

message("${COMPARE_OUTPUT}")

if(NOT COMPARE_RESULT EQUAL 0)
    message(FATAL_ERROR "Images do not match!\n${COMPARE_ERROR}")
endif()

message("Test PASSED: Images match within tolerance")
message("========================================")
