cmake_minimum_required(VERSION 3.23)
project(Lez1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        es_1/es_1.cpp
        es_1/es_1.h

        es_2/es_2.cpp
        es_2/es_2.h

        es_3/es_3.cpp
        es_3/es_3.h
)

add_executable(Lez1 main.cpp ${SOURCE_FILES})
