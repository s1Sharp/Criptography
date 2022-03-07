cmake_minimum_required(VERSION 3.9.1)
project(MPI)

set(CMAKE_CXX_STANDARD 14)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -pedantic -lm -O3 -funroll-loops")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

set(EXE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/executable")
set(EXECUTABLE_OUTPUT_PATH  ${EXE_DIR})


set(SOURCES main.cpp tasks.cpp utils.cpp)
add_executable(main ${SOURCES})