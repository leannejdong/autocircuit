cmake_minimum_required(VERSION 3.16)
project(autocircuit)

set(CMAKE_CXX_STANDARD 20)

add_executable(autocircuit main.cpp graph.h Mesh.h Mesh.cpp)