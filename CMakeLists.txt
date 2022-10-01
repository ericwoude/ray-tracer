cmake_minimum_required(VERSION 3.14)

project(
    simple-ray-tracer
    DESCRIPTION "An implementation of a basic ray tracer following Ray Tracing In One Weekend."
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_compile_options(-g)



include_directories(include)

add_executable(
    raytracer
    src/main.cpp
)
target_link_libraries(raytracer)

find_package (Threads REQUIRED)

target_link_libraries(raytracer
    ${CMAKE_THREAD_LIBS_INIT}
)