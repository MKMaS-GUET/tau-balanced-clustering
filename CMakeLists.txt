cmake_minimum_required(VERSION 3.17)
project(t_balance_clustering)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(tbc
                src/t_balance_clustering.cpp
                src/main.cpp)