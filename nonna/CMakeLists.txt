
cmake_minimum_required(VERSION 3.23)
project(Nonna)

configure_file(./dataset/input8.txt input.txt COPYONLY)

add_compile_definitions(LOCAL)

#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 11)

add_executable(Nonna1 "src/Nonna.cpp")
add_executable(Nonna2 src/Nonna2.cpp)
add_executable(Nonna3 src/Nonna3.cpp)
