cmake_minimum_required(VERSION 3.20)
project(BInt)

set(CMAKE_CXX_STANDARD 14)

add_executable(BInt main.cpp BigIntClass.h BigInt.cpp)
