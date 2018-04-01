project(cube)
cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

# Set global variables
set(CMAKE_BUILD_TYPE Debug)


# Turning off parts of GLFW we do not want
# Also I have deleted these folders from the repo,
# so we could not build them anymore if we wanted to - jonasjso 01.04.18
set(GLFW_BUILD_EXAMPLES OFF) 
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)

# Adding subdirectory, building the glfw library which we will
# link to later on.
add_subdirectory("${CMAKE_SOURCE_DIR}/lib/glfw-3.2.1")

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

###############################
# Microsoft Visual studio 2017
###############################
if(MSVC)

target_include_directories(
    cube 
    PRIVATE ${CMAKE_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/glew-2.1.0/include
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/glm-0.9.8.5
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/glfw-3.2.1/include
)

file(
    COPY ${CMAKE_SOURCE_DIR}/lib/glew-2.1.0/bin/glew32.dll 
    DESTINATION ${PROJECT_BINARY_DIR}/bin
)
file(
    COPY ${CMAKE_SOURCE_DIR}/lib/glew-2.1.0/lib/glew32.lib 
    DESTINATION ${PROJECT_BINARY_DIR}
)

target_link_libraries(
    cube
    PRIVATE ${OPENGL_LIBRARIES}
    PRIVATE glfw ${glfw_LIBRARIES} 
    PRIVATE glew32
)

################################
# Apple, linux, others...
################################
else()

target_compile_options(
    cube
    PRIVATE "-Wall"
    PRIVATE "-Wextra"
)

target_include_directories(
    cube
    PRIVATE ${CMAKE_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/glm-0.9.8.5
    PRIVATE ${CMAKE_SOURCE_DIR}/lib/glfw-3.2.1/include
)

target_link_libraries(
    cube
    PRIVATE ${OPENGL_LIBRARIES}
    PRIVATE glfw ${glfw_LIBRARIES} 
    PRIVATE ${GLEW_LIBRARY}
)
endif()


message("CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})