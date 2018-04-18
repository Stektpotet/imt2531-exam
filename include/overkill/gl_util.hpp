#pragma once
#include <iostream>
#include <string>
#include <GL/glew.h>

#include <PMS/logger.h>
#include <overkill/gl_caller.hpp>

GLuint GLTypeSize(const GLenum type);
std::string ShaderTypeName(const GLenum typeID);
void GLPrintMaxContants();
