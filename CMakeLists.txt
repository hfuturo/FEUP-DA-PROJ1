cmake_minimum_required(VERSION 3.17)
project(DA_PROJ1)

set(CMAKE_CXX_STANDARD 11)

add_executable(project source/main.cpp include/Network.h include/Station.h include/utils.h source/utils.cpp source/Network.cpp)