
cmake_minimum_required(VERSION 3.10)
project(LazyScript)
 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(DEBUG "Enable debug traces" OFF)
 
set(SOURCES
    src/lazyscript/parser.cpp
    src/lazyscript/lexer.cpp
    src/main.cpp
)
 
add_executable(LazyScript ${SOURCES})
 
target_include_directories(LazyScript PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
 
if(DEBUG)
    target_compile_definitions(LazyScript PRIVATE DEBUG_ENABLED=1)
    message(STATUS "Debug traces ENABLED")
else()
    message(STATUS "Debug traces disabled (use: cmake -DDEBUG=ON ..)")
endif()
 