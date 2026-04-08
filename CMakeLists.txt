
cmake_minimum_required(VERSION 3.10)
project(LazyScript)
 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LLVM_FIND_PACKAGE_NAME LLVM)
 
find_package(LLVM REQUIRED CONFIG)

message(STATUS "────────────────────────────────────────────────────────")
message(STATUS "LLVM Configuration:")
message(STATUS "────────────────────────────────────────────────────────")
message(STATUS "Found LLVM Version: ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM Install Prefix: ${LLVM_INSTALL_PREFIX}")
message(STATUS "LLVM Include Dir: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM Library Dir: ${LLVM_LIBRARY_DIRS}")
message(STATUS "LLVM Definitions: ${LLVM_DEFINITIONS}")
message(STATUS "────────────────────────────────────────────────────────")
 
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
 
set(LLVM_COMPONENTS
    Support          # LLVM General Support
    Core             # Basic IR
    ExecutionEngine  # JIT and execution
    MCJIT            # Modern JIT
    Interpreter     
    NativeCodeGen    
    Passes           # Pass infrastructure
    Analysis         
    Transforms       
    Vectorize        # Vectors optimization
    InstCombine      # Instructiions optimization
)
 
llvm_map_components_to_libnames(llvm_libs ${LLVM_COMPONENTS})
 
message(STATUS "LLVM Libraries: ${llvm_libs}")
message(STATUS "────────────────────────────────────────────────────────\n")

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
 