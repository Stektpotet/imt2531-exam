#include <overkill/gl_caller.hpp>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        LOG_WARN("[OpenGL Error]");
        std::cout << "[OpenGL Error] (" << error << ");\n" << function <<
            " in " << file << "\nat line: " << line << std::endl;
        std::cin.get();
        return true;
    }
    return false;
}