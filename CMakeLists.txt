cmake_minimum_required(VERSION 3.19)
project(CGSynch_2)
add_compile_options(-Wall -Wextra)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")

add_executable(CGSynch_2 main.cpp Game/CombinatorialGame.cpp Game/SynchronousGame.cpp Game/CombinatorialGame.h Game/SynchronousGame.h Game/CombinatorialGameDatabase.cpp Game/CombinatorialGameDatabase.h Scratch/temp.cc Game/util.h)