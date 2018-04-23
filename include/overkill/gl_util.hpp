#pragma once
#include <iostream>
#include <string>
#include <GL/glew.h>

#include <PMS/logger.h>
#include <overkill/gl_caller.hpp>

void GLPrintMaxConstants();
auto GLTypeSize(const GLenum type) -> GLsizei;
auto ShaderTypeName(const GLenum typeID) -> std::string;
