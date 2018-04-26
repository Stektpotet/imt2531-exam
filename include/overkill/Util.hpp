#pragma once

#include <sstream>
#include <fstream>
#include <string>
#include <utility>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <PMS/logger.h>
#include <overkill/MaterialSystem.hpp>
#include <overkill/ShaderSystem.hpp>
#include <overkill/Watcher.hpp>
#include <overkill/Config.hpp>

namespace overkill::Util 
{

auto fileToString(const std::string& filepath) -> std::string;  
auto fileToString(const std::string& filepath, std::string* shaderString) -> C::Err;  
void everyTwoSeconds(float t);
auto packNormal(float x, float y, float z) -> GLint;
auto packUV(float u, float v) -> GLushort;
void printMatrix(const glm::mat4& m, const std::string & name);
}
