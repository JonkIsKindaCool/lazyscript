cmake_minimum_required(VERSION 3.28)
project(LazyScript VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(LazyScript ${SOURCES})

target_include_directories(LazyScript PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)