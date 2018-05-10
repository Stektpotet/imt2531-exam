#include <overkill/gl_caller.hpp>


void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        LOG_WARN("\n[OpenGL Error] (%x);\n%s in %s\nat line %d", error, function, file, line);
        //std::cin.get();
        return true;
    }
    return false;
}

auto GLLogFirstError() -> overkill::C::Err
{
    if (GLenum error = glGetError())
    {
        LOG_WARN("\n[OpenGL Error] (%x);", error);
        std::cin.get();
        GLClearError();
        return 1;
    }
    return 0;
}

