cmake_minimum_required(VERSION 3.16)

project(find_sound_source CXX)

add_subdirectory(./mcp3208_pigpio)

add_compile_options(-Wall)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/mcp3208_pigpio/cmake/Modules/")

find_package(pigpio REQUIRED)

include_directories(
	${pigpio_INCLUDE_DIRS}
)

add_executable(get_sample get_sample.cpp)

target_include_directories(get_sample PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${pigpio_INCLUDE_DIRS} mcp3208_pigpio)
target_link_libraries(get_sample ${pigpio_LIBRARY} mcp3208_pigpio)