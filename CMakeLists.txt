project(cube)
cmake_minimum_required (VERSION 3.8)

# Set global variables
set(CMAKE_BUILD_TYPE Debug)

# Find system packages
find_package(OpenGL REQUIRED)
if(NOT MSVC)
	find_package(GLEW REQUIRED)
endif()

# Configure main executeable
add_executable(cube "${CMAKE_SOURCE_DIR}/src/main.cpp")

set_target_properties(
	cube
	PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

message("CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})