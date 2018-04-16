#include <overkill/ShaderSystem.hpp>

namespace overkill 
{

std::vector<ShaderProgram> ShaderSystem::m_shaderPrograms;
std::unordered_map<C::Tag, C::ID> ShaderSystem::m_mapShaderProgramID;


auto ShaderSystem::getIdByTag(const C::Tag& tag) -> C::ID
{
    return m_mapShaderProgramID[tag];
}

auto ShaderSystem::getByTag(const C::Tag& tag) -> const ShaderProgram&
{
    return ShaderSystem::m_shaderPrograms[m_mapShaderProgramID[tag]];
}

auto ShaderSystem::getById(C::ID shaderProgramID) -> const ShaderProgram&
{
    return ShaderSystem::m_shaderPrograms[shaderProgramID];
}

auto ShaderSystem::copyByTag(const C::Tag& tag) -> ShaderProgram 
{
    return ShaderSystem::m_shaderPrograms[m_mapShaderProgramID[tag]];
}

void ShaderSystem::push(const C::Tag&& tag, const char* path) 
{
    ShaderSystem::m_mapShaderProgramID[tag] = ShaderSystem::m_shaderPrograms.size();    
    ShaderSystem::m_shaderPrograms.emplace_back( ShaderProgram(path) );
}

void ShaderSystem::load() 
{
    ShaderSystem::push("base", "assets/shaders/base.shader");
    ShaderSystem::push("edge", "assets/shaders/edge.shader");
}

}