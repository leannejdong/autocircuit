cmake_minimum_required(VERSION 3.12)
project(circuit)

set(CMAKE_CXX_STANDARD 20)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_executable(circuit src/main.cpp include/util.h)
target_link_libraries (circuit Eigen3::Eigen)
