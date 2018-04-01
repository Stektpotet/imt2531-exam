#include <overkill/Init.hpp>

namespace overkill 
{

auto Init::GLFW(
    const int openglMajor, 
    const int openglMinor, 
    const int wwidth, 
    const int wheight, 
    const char* wname) -> GLFWwindow*
{
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

    GLFWwindow* window = glfwCreateWindow(wwidth, wheight, wname, NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL) {
        glfwTerminate();
        exit(-1); // LOG_ERROR(Init::logtag, "Failed to open GLFW window");
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSwapInterval(1);
    
    //q  glfwSetKeyCallback(window, key_callback);
    return window;
}

void Init::GLEW() 
{
#ifndef WIN32
    glewExperimental = GL_TRUE;  // Intel cpu graphics support for macOS/linux 
#endif

    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        exit(-1); // LOG_ERROR(Init::logtag, "Failed to init GLEW");
    }
}

void Init::OpenGL(const glm::vec4 background)
{
    glEnable(GL_PROGRAM_POINT_SIZE);
    glClearColor(background.x, background.y, background.z, background.w);

    glCullFace(GL_CCW); // GL_CCW | GL_CW

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

} // ::overkill