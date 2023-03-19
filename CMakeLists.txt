cmake_minimum_required(VERSION 3.17)
project(DA_PROJ1)

set(CMAKE_CXX_STANDARD 11)

add_executable(project source/main.cpp include/Graph.h source/Graph.cpp include/StationEdge.h source/StationEdge.cpp)