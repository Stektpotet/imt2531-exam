#include <iostream>
#include <string>
#include <vector>

#include <overkill/Init.hpp>

int main() 
{
    using namespace overkill;
    using BackgroundColor = glm::vec4;
    using VersionMajor = int;
    using VersionMinor = int;
    using Width        = int;
    using Height       = int;

	auto window = Init::GLFW(
        VersionMajor(4), VersionMinor(1), Width(800), Height(600), "Assignment 2 - Cube");

    Init::GLEW();
    Init::OpenGL( 
        BackgroundColor{ 1.0f, .8f, .6f, 1.0f});

    for(;;) 
    {
        if ((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window) != 0))
            break;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
	
	return 0;
}
