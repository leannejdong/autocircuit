cmake_minimum_required(VERSION 3.16)
project(circuit)

set(CMAKE_CXX_STANDARD 20)

add_executable(circuit main.cpp)

target_include_directories(circuit
        SYSTEM
        PUBLIC
        ${EIGEN3_INCLUDE_DIR}
        )
