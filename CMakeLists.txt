cmake_minimum_required(VERSION 3.10)
project(LazyScript)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(LLVM REQUIRED CONFIG)

# ←←← ESTA LÍNEA ES LA QUE SOLUCIONA EL ERROR
find_package(ZLIB REQUIRED)

message(STATUS "────────────────────────────────────────────────────────")
message(STATUS "LLVM Version: ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM Include Dir: ${LLVM_INCLUDE_DIRS}")
message(STATUS "────────────────────────────────────────────────────────")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Componentes mínimos (suficiente por ahora)
llvm_map_components_to_libnames(llvm_libs
    Core
    Support
    Analysis
)

message(STATUS "LLVM Libraries: ${llvm_libs}")
message(STATUS "────────────────────────────────────────────────────────\n")

option(DEBUG "Enable debug traces" OFF)

set(SOURCES
    src/main.cpp
    src/lazyscript/parser.cpp
    src/lazyscript/lexer.cpp
    src/lazyscript/llvm_gen.cpp
)

add_executable(LazyScript ${SOURCES})

target_include_directories(LazyScript PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(LazyScript PRIVATE ${llvm_libs} ZLIB::ZLIB)

if(DEBUG)
    target_compile_definitions(LazyScript PRIVATE DEBUG_ENABLED=1)
    message(STATUS "Debug traces ENABLED")
else()
    message(STATUS "Debug traces disabled (use: cmake -DDEBUG=ON ..)")
endif()