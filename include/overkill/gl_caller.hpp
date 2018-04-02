#pragma once

#include <GL/glew.h>
#include <iostream>

#define ASSERT(glFunc) if (!(glFunc)) __debugbreak();
#define GLCall(glFunc) GLClearError();\
    glFunc;\
    ASSERT(GLLogCall(#glFunc, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);