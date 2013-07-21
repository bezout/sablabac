cmake_minimum_required(VERSION 2.8)

project(sablabac)

#find_package(Eigen REQUIRED PATH /home/datta/develop/root/include/eigen3)
include_directories(${CMAKE_PREFIX_PATH})

set(CMAKE_CXX_COMPILER g++-4.8)
set(CMAKE_CXX_FLAGS -std=c++0x)

add_executable(file file.cpp)
target_link_libraries(file cxsparse)
