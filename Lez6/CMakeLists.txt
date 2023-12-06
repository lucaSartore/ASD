cmake_minimum_required(VERSION 3.23)
project(Lez2)

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 11)

add_executable(Componente Componente/componente.cpp)

add_executable(TopologicalOrder TopologicalOrder/topological_order.cpp)

add_executable(CamminoMax CamminoMax/cammina_max.cpp dividi_et_impera/main.cpp)

add_executable(CicliCiclabili CicliCiclabili/cicli_ciclabili.cpp)
add_executable(CicliCiclabili2 CicliCiclabili/cicli_ciclabili_2.cpp)
add_executable(CicliCiclabili3 CicliCiclabili/cicli_ciclabili_3.cpp)

add_executable(DividiEtImpera dividi_et_impera/main.cpp)

add_executable(AmongASD AmongASD/amongasd.cpp)