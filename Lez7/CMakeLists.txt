cmake_minimum_required(VERSION 3.23)
project(Lez7)

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 11)

add_executable(Zaino zaino/zaino.cpp)

add_executable(sottoinscresc "Sottoinsieme Creschente/sottoinscresc.cpp")
add_executable(sottoinscrescV2 "Sottoinsieme Creschente/sottoinscrescV2.cpp")

add_executable(Zaino2 zaino/zaino_2.cpp)

add_executable(Pillole Pillole/pillole.cpp)