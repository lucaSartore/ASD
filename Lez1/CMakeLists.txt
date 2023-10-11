cmake_minimum_required(VERSION 3.23)
project(Lez1)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        es_1/es_1.cpp
        es_1/es_1.h

        es_2/es_2.cpp
        es_2/es_2.h

        es_3/es_3.cpp
        es_3/es_3.h
)

#add_executable(Lez1 main.cpp)
add_executable(Sottomat sottomat.cpp)
add_executable(Lez1 main.cpp ${SOURCE_FILES})
add_executable(Cestini grader.cpp cestini.h cesrtini.cpp)
add_executable(Brackets brackets.cpp)
add_executable(Brackets2 brackets2.cpp)
add_executable(Sottovec sottovet.cpp)
