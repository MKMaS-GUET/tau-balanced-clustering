cmake_minimum_required(VERSION 3.17)
project(tau_balanced_clustering)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(include)

add_executable(tbc
                src/t_balance_clustering.cpp
                src/main.cpp)