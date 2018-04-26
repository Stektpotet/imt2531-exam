#include <overkill/gl_caller.hpp>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ");\n" << function <<
            " in " << file << "\nat line: " << line << std::endl;
        //std::cin.get();
        return true;
    }
    return false;
}

auto GLLogFirstError() -> overkill::C::Err
{
    if (GLenum error = glGetError())
    {
        LOG_WARN("[OpenGL Error] (%d);", error);
        std::cin.get();
        GLClearError();
        return 1;
    }
    return 0;
}