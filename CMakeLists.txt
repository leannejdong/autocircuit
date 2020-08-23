cmake_minimum_required(VERSION 3.16)
project(circuit)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

set(CMAKE_CXX_STANDARD 20)

add_executable(circuit main.cpp)

#target_include_directories(circuit
#        SYSTEM
#        PUBLIC
#        ${EIGEN3_INCLUDE_DIR}
#        )

target_link_libraries (circuit Eigen3::Eigen)
