cmake_minimum_required(VERSION 3.5.0)
project(raytracer VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
if(BUILD_TESTING)
  find_package(Python3 COMPONENTS Interpreter REQUIRED)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(raytracer main.cpp)

target_include_directories(raytracer PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/src/raymath"
                           "${PROJECT_SOURCE_DIR}/src/rayimage"
                           "${PROJECT_SOURCE_DIR}/src/rayscene"
                           )

add_subdirectory(./src/raymath)
add_subdirectory(./src/rayimage)
add_subdirectory(./src/rayscene)
add_subdirectory(./src/lodepng)

target_link_libraries(raytracer 
                      PUBLIC 
                      rayscene
                      raymath
                      rayimage
                      lodepng
                      )

if(BUILD_TESTING)
  set(TEST_ARTIFACT_DIR "${CMAKE_BINARY_DIR}/test-artifacts")
  file(MAKE_DIRECTORY "${TEST_ARTIFACT_DIR}")

  set(E2E_HELPER "${PROJECT_SOURCE_DIR}/tests/e2e/run_raytrace_and_compare.py")

  add_test(
    NAME raytracer_e2e_basic_scene
    COMMAND "${Python3_EXECUTABLE}" "${E2E_HELPER}"
            --binary "$<TARGET_FILE:raytracer>"
            --scene "${PROJECT_SOURCE_DIR}/scenes/tests/basic.json"
            --expected "${PROJECT_SOURCE_DIR}/tests/baseline/basic.png"
            --output "${TEST_ARTIFACT_DIR}/basic.png"
  )
  set_tests_properties(raytracer_e2e_basic_scene PROPERTIES LABELS "e2e;regression")

  add_test(
    NAME raytracer_e2e_empty_scene
    COMMAND "${Python3_EXECUTABLE}" "${E2E_HELPER}"
            --binary "$<TARGET_FILE:raytracer>"
            --scene "${PROJECT_SOURCE_DIR}/scenes/tests/empty.json"
            --expected "${PROJECT_SOURCE_DIR}/tests/baseline/empty.png"
            --output "${TEST_ARTIFACT_DIR}/empty.png"
  )
  set_tests_properties(raytracer_e2e_empty_scene PROPERTIES LABELS "e2e;edge-case")
endif()

