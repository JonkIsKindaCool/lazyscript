cmake_minimum_required(VERSION 3.28)
project(LazyScript VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(LazyScript main.cpp)
target_include_directories(LazyScript PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")