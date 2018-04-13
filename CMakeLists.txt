project(cube)
cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

# Set global variables
set(CMAKE_BUILD_TYPE Debug)

# Aliases
set(SRCDIR ${CMAKE_SOURCE_DIR}) 
set(BINDIR ${CMAKE_BINARY_DIR})


# Find system packages
find_package(OpenGL REQUIRED)
if(NOT MSVC)
    find_package(GLEW REQUIRED)
endif()

# Turning off parts of GLFW we do not want
# Also I have deleted these folders from the repo,
# so we could not build them anymore if we wanted to - jonasjso 01.04.18
set(GLFW_BUILD_EXAMPLES 0)
set(GLFW_BUILD_TESTS 0)
set(GLFW_BUILD_DOCS 0)

# Adding subdirectory, building the glfw library which we will
# link to later on.
add_subdirectory("${SRCDIR}/lib/glfw-3.2.1")
add_subdirectory("${SRCDIR}/lib/SOIL-1.07")


# Configure main executeable
add_executable(cube "${SRCDIR}/src/main.cpp")

set_target_properties(
    cube
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY "${BINDIR}"
    LIBRARY_OUTPUT_DIRECTORY "${BINDIR}/bin"
)

target_sources(
    cube
    PRIVATE ${SRCDIR}/src/Init.cpp
	PRIVATE ${SRCDIR}/src/gl_caller.cpp
    PRIVATE ${SRCDIR}/src/vertexArray.cpp
    PRIVATE ${SRCDIR}/src/vertexBuffer.cpp
    PRIVATE ${SRCDIR}/src/renderer.cpp
    PRIVATE ${SRCDIR}/src/texture.cpp
    PRIVATE ${SRCDIR}/src/shader.cpp
    PRIVATE ${SRCDIR}/src/gl_util.cpp
    PRIVATE ${SRCDIR}/src/elementBuffer.cpp
)

# Asssets which frequently updates
configure_file(${SRCDIR}/assets/shaders/base.shader ${BINDIR}/assets/shaders/base.shader)
configure_file(${SRCDIR}/assets/shaders/edge.shader ${BINDIR}/assets/shaders/edge.shader)
configure_file(${SRCDIR}/assets/models/cube-inside-cube.yml ${BINDIR}/assets/models/cube-inside-cube.yml)


###############################
# Microsoft Visual studio 2017
###############################
if(MSVC)

target_include_directories(
    cube
    PRIVATE ${SRCDIR}
    PRIVATE ${SRCDIR}/include
    PRIVATE ${SRCDIR}/lib/glew-2.1.0/include
    PRIVATE ${SRCDIR}/lib/glm-0.9.8.5
    PRIVATE ${SRCDIR}/lib/glfw-3.2.1/include
    PRIVATE ${SRCDIR}/lib/SOIL-1.07/src
)

file(
    COPY ${SRCDIR}/lib/glew-2.1.0/bin/glew32.dll
    DESTINATION ${PROJECT_BINARY_DIR}/bin
)
file(
    COPY ${SRCDIR}/lib/glew-2.1.0/lib/glew32.lib
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
    PRIVATE ${SRCDIR}
    PRIVATE ${SRCDIR}/include
    PRIVATE ${SRCDIR}/lib/glm-0.9.8.5
    PRIVATE ${SRCDIR}/lib/glfw-3.2.1/include
    PRIVATE ${SRCDIR}/lib/SOIL-1.07/src
)

target_link_libraries(
    cube
    PRIVATE ${OPENGL_LIBRARIES}
    PRIVATE glfw ${glfw_LIBRARIES}
    PRIVATE ${GLEW_LIBRARY}
    PRIVATE SOIL ${SOIL_LIBRARIES}

)
endif()



add_custom_command(
    TARGET cube POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SRCDIR}/assets $<TARGET_FILE_DIR:cube>/assets
)

message("CMAKE_BINARY_DIR: " ${BINDIR})
message("OPENGL_LIBRARIES"   ${OPENGL_LIBRARIES})
message("GLFW_LIBRARIES"     ${GLFW_LIBRARIES})
message("GLEW_LIBRARY"       ${GLEW_LIBRARY})
message("SOIL_LIBRARIES"     ${SOIL_LIBRARIES})