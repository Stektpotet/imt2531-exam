#pragma once

#include <GL/glew.h>
#include <iostream>
#include <PMS/logger.h>
#include <overkill/Config.hpp>



#if _MSC
    #define ASSERT(notglerr) if (!(notglerr)) __debugbreak();
#else
    #include <exception>
    #define ASSERT(notglerr) if (!(notglerr)) std::terminate();
#endif
#define GLCall(glFunc) GLClearError();\
    glFunc;\
    ASSERT(!GLLogCall(#glFunc, __FILE__, __LINE__))


#define GLCall_ReturnIfError(glFunc) GLClearError();\
    glFunc;\
    err = GLLogCall(#glFunc, __FILE__, __LINE__);\
    if (err)\
        return err;



void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
auto GLLogFirstError() -> overkill::C::Err;


