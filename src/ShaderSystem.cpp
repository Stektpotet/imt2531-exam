#include <overkill/ShaderSystem.hpp>

namespace overkill 
{

std::vector<ShaderProgram> ShaderSystem::m_shaderPrograms;
std::unordered_map<C::Tag, C::ID> ShaderSystem::m_mapShaderProgramID;
std::vector<ShaderSystem::UpdateCallback> ShaderSystem::m_updateCallbacks;


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

auto ShaderSystem::copyById(C::ID shaderProgramID) -> ShaderProgram
{
    return ShaderSystem::m_shaderPrograms[shaderProgramID];
}

void ShaderSystem::push(const C::Tag&& tag, const char* path) 
{
    ShaderSystem::m_mapShaderProgramID[tag] = ShaderSystem::m_shaderPrograms.size();    
    ShaderSystem::m_shaderPrograms.emplace_back( ShaderProgram(path) );
}

void ShaderSystem::load() 
{
    ShaderSystem::push("base", "assets/shaders/base.glsl");
    ShaderSystem::push("default", "assets/shaders/default.glsl");
}

void ShaderSystem::reload() 
{

    // Delete from GPU
    for (auto shaderprog : ShaderSystem::m_shaderPrograms) 
    {
        shaderprog.clean();         
    }

    // Remove all shaders
    ShaderSystem::m_shaderPrograms.clear();

    // Load from file again
    ShaderSystem::load();
    for(auto onUpdate : ShaderSystem::m_updateCallbacks)
    {
        auto shaderID =  ShaderSystem::getIdByTag(onUpdate.tag);
        onUpdate.callback(shaderID, onUpdate.modelID, onUpdate.meshID);
    }
}

void ShaderSystem::bindOnUpdate(const C::Tag& shaderTag, C::ID modelID, C::ID meshID, OnUpdate onUpdate)
{
    ShaderSystem::m_updateCallbacks.emplace_back(
        UpdateCallback{ 
            shaderTag,
            modelID,
            meshID, 
            onUpdate 
        }
    );
}

void ShaderSystem::unbindAll()
{
    ShaderSystem::m_updateCallbacks.clear();
}



}
