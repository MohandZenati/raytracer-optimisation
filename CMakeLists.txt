cmake_minimum_required(VERSION 3.5.0)
project(raytracer VERSION 0.1.0 LANGUAGES C CXX)

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

# Enable CTest
enable_testing()

# Test executable for rendering and image comparison
add_executable(test_render tests/test_render.cpp)

target_include_directories(test_render PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/src/raymath"
                           "${PROJECT_SOURCE_DIR}/src/rayimage"
                           "${PROJECT_SOURCE_DIR}/src/rayscene"
                           )

target_link_libraries(test_render
                      PUBLIC
                      rayscene
                      raymath
                      rayimage
                      lodepng
                      )

# Add CTest test
add_test(NAME render_and_compare
         COMMAND test_render
         WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
