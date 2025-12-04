# Script to generate reference images for all test scenes

if(NOT DEFINED RAYTRACER)
    message(FATAL_ERROR "RAYTRACER not defined")
endif()

if(NOT DEFINED REFERENCE_DIR)
    message(FATAL_ERROR "REFERENCE_DIR not defined")
endif()

if(NOT DEFINED TEST_SCENES_DIR)
    message(FATAL_ERROR "TEST_SCENES_DIR not defined")
endif()

# Create references directory if it doesn't exist
file(MAKE_DIRECTORY ${REFERENCE_DIR})

message("========================================")
message("Generating reference images")
message("  Raytracer: ${RAYTRACER}")
message("  Output:    ${REFERENCE_DIR}")
message("========================================")

# List of test scenes
set(TEST_SCENES
    "simple-test"
    "edge-case-empty"
    "edge-case-single-sphere"
    "complex-test"
)

# Generate reference for each scene
foreach(SCENE_NAME ${TEST_SCENES})
    set(SCENE_FILE "${TEST_SCENES_DIR}/${SCENE_NAME}.json")
    # Convert scene name: replace hyphens with underscores for reference filename
    string(REPLACE "-" "_" OUTPUT_NAME "${SCENE_NAME}")
    set(OUTPUT_FILE "${REFERENCE_DIR}/${OUTPUT_NAME}.png")
    
    message("Generating reference for: ${SCENE_NAME}")
    
    if(NOT EXISTS ${SCENE_FILE})
        message(WARNING "Scene file not found: ${SCENE_FILE}")
        continue()
    endif()
    
    execute_process(
        COMMAND ${RAYTRACER} ${SCENE_FILE} ${OUTPUT_FILE}
        RESULT_VARIABLE RESULT
        OUTPUT_VARIABLE OUTPUT
        ERROR_VARIABLE ERROR
    )
    
    if(NOT RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to generate reference for ${SCENE_NAME}\nError: ${ERROR}")
    endif()
    
    message("  Generated: ${OUTPUT_FILE}")
    message("${OUTPUT}")
endforeach()

message("========================================")
message("All reference images generated successfully!")
message("========================================")
