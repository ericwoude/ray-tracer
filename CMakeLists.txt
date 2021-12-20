cmake_minimum_required(VERSION 3.14)

project(
    cpp-template
    DESCRIPTION "An implementation of a basic ray tracer following Ray Tracing In One Weekend."
    VERSION 0.0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(include)

add_executable(
    raytracer
    src/main.cpp
)
target_link_libraries(raytracer)