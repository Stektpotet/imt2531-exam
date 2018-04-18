#include <overkill/ShaderSystem.hpp>

namespace overkill 
{
std::vector<DynamicUniformBuffer>           ShaderSystem::m_uBuffersDynamic;
std::unordered_map<C::Tag, C::ID>           ShaderSystem::m_mapUniformBufferDynamic;
std::vector<ShaderProgram>                  ShaderSystem::m_shaderPrograms;
std::unordered_map<C::Tag, C::ID>           ShaderSystem::m_mapShaderProgramID;
std::vector<ShaderSystem::UpdateCallback>   ShaderSystem::m_updateCallbacks;

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

void ShaderSystem::pushUniformBuffer(const C::Tag&& tag, GLuint size)
{
    ShaderSystem::m_mapUniformBufferDynamic[tag] = ShaderSystem::m_uBuffersDynamic.size();
    ShaderSystem::m_uBuffersDynamic.emplace_back( DynamicUniformBuffer(tag.c_str(), nullptr, size) );
}


auto ShaderSystem::getUniformBufferIdByTag(const C::Tag& tag) -> C::ID
{
    return m_mapUniformBufferDynamic[tag];
}

auto ShaderSystem::getUniformBufferByTag(const C::Tag& tag) -> const DynamicUniformBuffer&
{
    return ShaderSystem::m_uBuffersDynamic[m_mapShaderProgramID[tag]];
}

auto ShaderSystem::getUniformBufferById(C::ID uBufferID) -> const DynamicUniformBuffer&
{
    return ShaderSystem::m_uBuffersDynamic[uBufferID];
}



void ShaderSystem::load() 
{

    ShaderSystem::push("base", "assets/shaders/base.shader");
    //ShaderSystem::push("edge", "assets/shaders/edge.shader");
    
    pushUniformBuffer("OK_Lights", sizeof(LightData) * 8);
    pushUniformBuffer("OK_Matrices", sizeof(MVP));

    //m_uBuffersDynamic.emplace_back( DynamicUniformBuffer("OK_Lights",nullptr, sizeof(LightData)*8) );
    //m_uBuffersDynamic.emplace_back( DynamicUniformBuffer("OK_Matrices",nullptr, sizeof(MVP)) );

    linkUniformBlocksForAll();
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

void ShaderSystem::linkUniformBlocksForAll()
{   
    GLuint bindPoint = 0;
    for (auto& uBuffer : m_uBuffersDynamic)
    {
        for (auto& shader : m_shaderPrograms)
        {
            GLCall(glUniformBlockBinding(GLuint(shader), shader.getUniformBlockIndex(C::Tag(uBuffer)), bindPoint));
            GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, GLuint(uBuffer)));
        }
        bindPoint++;
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

}
