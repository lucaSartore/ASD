cmake_minimum_required(VERSION 3.23)
project(Lez7)

add_compile_definitions(LOCAL)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 11)


add_executable( tsp "tsp/tsp.cpp")
add_executable( tsp2 "tsp/tsp2.cpp")
add_executable( tsp3 "tsp/tsp3.cpp")
add_executable( alpini "alpini/alpini.cpp")

