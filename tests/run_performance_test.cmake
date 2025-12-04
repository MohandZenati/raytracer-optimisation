# Script to run a performance test
# This script measures the execution time of the raytracer

if(NOT DEFINED RAYTRACER)
    message(FATAL_ERROR "RAYTRACER not defined")
endif()

if(NOT DEFINED SCENE_FILE)
    message(FATAL_ERROR "SCENE_FILE not defined")
endif()

if(NOT DEFINED OUTPUT_IMAGE)
    message(FATAL_ERROR "OUTPUT_IMAGE not defined")
endif()

message("========================================")
message("Running performance test")
message("  Scene:  ${SCENE_FILE}")
message("  Output: ${OUTPUT_IMAGE}")
message("========================================")

# Run the raytracer multiple times to get average
set(NUM_RUNS 3)
set(TOTAL_TIME 0)

message("Running ${NUM_RUNS} iterations...")

foreach(RUN RANGE 1 ${NUM_RUNS})
    message("Run ${RUN}/${NUM_RUNS}...")
    
    execute_process(
        COMMAND ${RAYTRACER} ${SCENE_FILE} ${OUTPUT_IMAGE}
        RESULT_VARIABLE RENDER_RESULT
        OUTPUT_VARIABLE RENDER_OUTPUT
        ERROR_VARIABLE RENDER_ERROR
    )
    
    if(NOT RENDER_RESULT EQUAL 0)
        message(FATAL_ERROR "Raytracer failed on run ${RUN}")
    endif()
    
    # Extract time from output (assumes format "Total time: X.XXX seconds.")
    string(REGEX MATCH "Total time: ([0-9]+\\.[0-9]+) seconds" TIME_MATCH "${RENDER_OUTPUT}")
    if(TIME_MATCH)
        set(RUN_TIME ${CMAKE_MATCH_1})
        message("  Time: ${RUN_TIME} seconds")
        math(EXPR TOTAL_TIME "${TOTAL_TIME} + ${RUN_TIME}")
    endif()
endforeach()

# Calculate average
math(EXPR AVG_TIME "${TOTAL_TIME} / ${NUM_RUNS}")

message("========================================")
message("Performance Results:")
message("  Number of runs: ${NUM_RUNS}")
message("  Average time:   ${AVG_TIME} seconds")
message("========================================")

# Performance test always passes - it's just for metrics
