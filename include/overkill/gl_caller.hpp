#pragma once

#include <GL/glew.h>
#include <iostream>

#if _MSC
    #define ASSERT(glFunc) if (!(glFunc)) __debugbreak();
#else
    #include <exception>
    #define ASSERT(glFunc) if (!(glFunc)) std::terminate();
#endif
#define GLCall(glFunc) GLClearError();\
    glFunc;\
    ASSERT(GLLogCall(#glFunc, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
