cmake_minimum_required(VERSION 3.11.4 FATAL_ERROR)

option(BUILD_STATIC_LIBRARY "Build static library" OFF)

set(CMAKE_TOOLCHAIN_FILE "ARMToolchain.cmake")

project(matrix_f32_library C CXX)

if(BUILD_STATIC_LIBRARY)
    include_directories(${CMAKE_SOURCE_DIR})
    add_library(matrix_f32 STATIC src/matrix_f32.c)
endif()
