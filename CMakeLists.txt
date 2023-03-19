cmake_minimum_required(VERSION 3.17)
project(DA_PROJ1)

set(CMAKE_CXX_STANDARD 11)

add_executable(project source/main.cpp include/Network.h source/Network.cpp include/Station.h source/Station.cpp include/utils.h source/utils.cpp include/Graph.h source/Graph.cpp)