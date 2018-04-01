#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

auto init_GLFW_GLEW(
    const int openglMajor, 
    const int openglMinor, 
    const int wwidth, 
    const int wheight, 
    const char* wname) -> GLFWwindow*
{
    // init G L F W
    if (!glfwInit()){
        glfwTerminate();
		exit(-1); //(Init::logtag, "Failed to init GLFW");
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openglMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openglMinor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // OPEN G L F W WINDOW AND HOOK UP THE OPEN GL CONTEXT
    //
    GLFWwindow* window = glfwCreateWindow(wwidth, wheight, wname, NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL) {
        glfwTerminate();
		exit(-1); // LOG_ERROR(Init::logtag, "Failed to open GLFW window");
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSwapInterval(1);

    // init G L E W
    //

#ifndef WIN32
    glewExperimental = GL_TRUE;  // MACOS/intel cpu support
#endif

	if (glewInit() != GLEW_OK) {
        glfwTerminate();
		exit(-1); // LOG_ERROR(Init::logtag, "Failed to init GLEW");
    }
  //q  glfwSetKeyCallback(window, key_callback);
    return window;
}


int main() 
{
	std::cout << "Hello sailor!\n";
	
	auto window = init_GLFW_GLEW(4,1, 800, 600, "Assignment 2");


	
	std::cin.get();
	return 0;
}
