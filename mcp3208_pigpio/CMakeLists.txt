cmake_minimum_required(VERSION 3.16)

project(mcp3208_pigpio CXX)

add_compile_options(-Wall)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

find_package(pigpio REQUIRED)

set(library_name mcp3208_pigpio)

set(mcp3208_pigpio_INCLUDE_DIRS ./include/mcp3208_pigpio)

add_library(${library_name} STATIC
  src/mcp3208.cpp
)

target_include_directories(${library_name} INTERFACE ${pigpio_INCLUDE_DIRS} ${mcp3208_pigpio_INCLUDE_DIRS})