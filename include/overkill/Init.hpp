#pragma once

#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <PMS/logger.h>

#include <overkill/gl_caller.hpp>
#include <overkill/Parser.hpp>
#include <overkill/Input.hpp>

#include <overkill/ModelSystem.hpp>
#include <overkill/MaterialSystem.hpp>
#include <overkill/TextureSystem.hpp>


namespace overkill::Init
{

auto GLFW(
    const int openglMajor, 
    const int openglMinor, 
    const int wwidth, 
    const int wheight, 
    const char* wname) -> GLFWwindow*;
void GLEW();
void OpenGL(const glm::vec4 background);

void Config();
void loadOBJFiles(bool loadTextures=true, bool loadMaterials=true, bool loadModels=true);

} // ::Init::overkill
