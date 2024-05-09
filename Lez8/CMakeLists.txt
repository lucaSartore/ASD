cmake_minimum_required(VERSION 3.23)
project(Lez7)

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 11)


add_executable(MinCover "MinCover/main.cpp")
add_executable(MinCoverPesata "MinCoverPesata /main.cpp")
add_executable(LCS "LCS/main.cpp")
add_executable(Lich "Lich/main.cpp")
add_executable(Lich3 "Lich/main3.cpp")
add_executable(Lich4 "Lich/main4.cpp")
add_executable(Nassau "Nassau/main.cpp")

