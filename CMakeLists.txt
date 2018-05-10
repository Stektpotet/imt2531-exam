project(cube)
cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

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
add_subdirectory("${SRCDIR}/lib/tiny_obj_loader")


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
    PRIVATE ${SRCDIR}/src/Config.cpp
    PRIVATE ${SRCDIR}/src/ElementBuffer.cpp
	PRIVATE ${SRCDIR}/src/gl_caller.cpp
    PRIVATE ${SRCDIR}/src/gl_util.cpp
    PRIVATE ${SRCDIR}/src/Entity.cpp
    PRIVATE ${SRCDIR}/src/EntityNode.cpp    
    PRIVATE ${SRCDIR}/src/EntityModel.cpp
    PRIVATE ${SRCDIR}/src/EntityCamera.cpp
    PRIVATE ${SRCDIR}/src/EntityPointLight.cpp
    PRIVATE ${SRCDIR}/src/EntityDirectionalLight.cpp
    PRIVATE ${SRCDIR}/src/Init.cpp
    PRIVATE ${SRCDIR}/src/Input.cpp
    PRIVATE ${SRCDIR}/src/Material.cpp
    PRIVATE ${SRCDIR}/src/MaterialSystem.cpp
    PRIVATE ${SRCDIR}/src/Model.cpp
    PRIVATE ${SRCDIR}/src/ModelSystem.cpp
    PRIVATE ${SRCDIR}/src/Parser.cpp
    PRIVATE ${SRCDIR}/src/Renderer.cpp
    PRIVATE ${SRCDIR}/src/Scene.cpp    
    PRIVATE ${SRCDIR}/src/ShaderProgram.cpp
    PRIVATE ${SRCDIR}/src/ShaderSystem.cpp
    PRIVATE ${SRCDIR}/src/Texture.cpp
    PRIVATE ${SRCDIR}/src/TextureSystem.cpp
    PRIVATE ${SRCDIR}/src/Util.cpp
    PRIVATE ${SRCDIR}/src/VertexArray.cpp
    PRIVATE ${SRCDIR}/src/VertexBuffer.cpp
    PRIVATE ${SRCDIR}/src/Watcher.cpp

	PRIVATE ${SRCDIR}/src/UniformBuffer.cpp
	PRIVATE ${SRCDIR}/src/ShaderIntrospector.cpp
)

#
# CONFIGURE DIRECTORIES (configure_file())
#
function(configure_directory src dest)

    file(GLOB files ${src}/*)
    foreach(absolutepath ${files})
        get_filename_component(filename ${absolutepath} NAME)
        message("Conf file: " ${src}/${filename} " -> " ${dest}/${filename})
        configure_file(${src}/${filename} ${dest}/${filename} COPYONLY)
    endforeach(absolutepath)

endfunction()

configure_directory(${SRCDIR}/assets/scenes ${BINDIR}/assets/scenes)
configure_directory(${SRCDIR}/assets/materials ${BINDIR}/assets/materials)
configure_directory(${SRCDIR}/assets/shaders ${BINDIR}/assets/shaders)
configure_directory(${SRCDIR}/assets/models ${BINDIR}/assets/models)
configure_directory(${SRCDIR}/assets/textures ${BINDIR}/assets/textures)

configure_directory(${SRCDIR}/assets/objs/house ${BINDIR}/assets/objs/house)
configure_directory(${SRCDIR}/assets/objs/cube ${BINDIR}/assets/objs/cube)


configure_file(${SRCDIR}/tool/discover.py ${BINDIR}/tool/discover.py COPYONLY)
configure_file(${SRCDIR}/config.yml ${BINDIR}/config.yml COPYONLY)


###############################
# Microsoft Visual studio 2017
###############################
if(MSVC)

target_include_directories(
    cube
    PRIVATE ${SRCDIR}
    PRIVATE ${SRCDIR}/include
    PRIVATE ${SRCDIR}/lib/
    PRIVATE ${SRCDIR}/lib/glew-2.1.0/include
    PRIVATE ${SRCDIR}/lib/glm-0.9.8.5
    PRIVATE ${SRCDIR}/lib/glfw-3.2.1/include
    PRIVATE ${SRCDIR}/lib/SOIL-1.07/src
)

file(
    COPY ${SRCDIR}/lib/glew-2.1.0/bin/glew32.dll
    DESTINATION ${PROJECT_BINARY_DIR}
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
	PRIVATE SOIL
    PRIVATE tinyobjloader
)

################################
# Apple, linux, others...
################################
else()

target_compile_options(
    cube
    PRIVATE "-Wall"
    PRIVATE "-Wextra"
    PRIVATE "-std=c++17"
)

target_include_directories(
    cube
    PRIVATE ${SRCDIR}
    PRIVATE ${SRCDIR}/include
    PRIVATE ${SRCDIR}/lib/
    PRIVATE ${SRCDIR}/lib/glm-0.9.8.5
    PRIVATE ${SRCDIR}/lib/glfw-3.2.1/include
    PRIVATE ${SRCDIR}/lib/SOIL-1.07/src
)

target_link_libraries(
    cube
    PRIVATE ${OPENGL_LIBRARIES}
    PRIVATE ${GLEW_LIBRARY}
    PRIVATE glfw
    PRIVATE SOIL
    PRIVATE tinyobjloader
)
endif()

# # Test discover.py and test-discover.cpp
# add_executable(test_discover "${SRCDIR}/tool/test-discover.cpp")
# set_target_properties(
#     test_discover
#     PROPERTIES
#     CXX_STANDARD 17
#     CXX_STANDARD_REQUIRED ON
#     RUNTIME_OUTPUT_DIRECTORY "${BINDIR}"
# )



message("CMAKE_BINARY_DIR: " ${BINDIR})
message("OPENGL_LIBRARIES"   ${OPENGL_LIBRARIES})
message("GLEW_LIBRARY"       ${GLEW_LIBRARY})
