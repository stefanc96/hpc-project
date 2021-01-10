cmake_minimum_required(VERSION 3.17)
project(hpc_project C)

set(CMAKE_C_STANDARD 99)
find_package(MPI REQUIRED)
include_directories(/usr/local/include)

add_executable(hpc_project root.c worker.c)