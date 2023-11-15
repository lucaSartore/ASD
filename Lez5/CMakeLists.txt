cmake_minimum_required(VERSION 3.23)
project(Lez2)

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 11)

add_executable(Visita Visita/Visita.cpp)
add_executable(Diametro Diametro/Diametro.cpp)
