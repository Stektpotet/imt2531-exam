#pragma once

#include <set>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <array>
#include <iostream>

#include <overkill/Config.hpp>
#include <overkill/ShaderProgram.hpp>
#include <overkill/UniformBuffer.hpp>
#include <overkill/ShaderIntrospector.hpp>
#include <overkill/Watcher.hpp>
#include <overkill/Util.hpp>

namespace overkill
{

class ShaderSystem 
{
public:
    using OnUpdate = void (*)(C::ID, C::ID, C::ID);
    struct UpdateCallback 
    {
        C::Tag   tag;
        C::ID    modelID;
        C::ID    meshID;
        OnUpdate callback;
    };
private:


    static std::unordered_map<C::Tag, std::set<GLuint>> m_mapUniformBufferTargets;
    static std::vector<UniformBuffer>           m_uniformBuffers;
    static std::unordered_map<C::Tag, C::ID>    m_mapUniformBuffersID;

    static std::vector<ShaderProgram>           m_shaderPrograms;
    static std::unordered_map<C::Tag, C::ID>    m_mapShaderProgramID;

    static std::vector<UpdateCallback>          m_updateCallbacks;

    static auto parseProgram(const std::string& file, 
                             std::string* vert, 
                             std::string* frag, 
                             std::string* geom,
                             std::array<std::vector<GLenum>, 3>* drawProperties) -> C::Err;

    static auto makeProgram(const std::string& vert, 
                            const std::string& frag, 
                            const std::string& geom,
                            const std::array<std::vector<GLenum>, 3>& drawProperties,
                            ShaderProgram* program) -> C::Err;

    static void push(const C::Tag tag, const std::string& filepath);
    static void pushUniformBuffer(const C::Tag&& tag, GLuint size);
    static void linkUniformBlocks();
    static void enlistUniformBlockTargets();

public:
    // <summary> Load all shader data onto GPU memory. 
    //           Keep shader metadata on CPU </summary>
    static void createUniformBuffers();
    static void load();
    static void reload();
    static void clean();
    static auto getIdByTag(const C::Tag& shaderTag) -> C::ID;
    static auto getByTag(const C::Tag& shaderTag) -> const ShaderProgram&;
    static auto getById(const C::ID shaderProgramID ) -> const ShaderProgram&;
    static auto copyByTag(const C::Tag& tag) -> ShaderProgram;
    static auto copyById(C::ID shaderProgramID) -> ShaderProgram;
    static void bindOnUpdate(const C::Tag& shaderTag, C::ID modelID, C::ID meshID, OnUpdate onUpdate);
    
    static void unbindAll();

    static auto getUniformBufferIdByTag(const C::Tag& tag)->C::ID;
    static auto getUniformBufferByTag(const C::Tag& tag) -> const UniformBuffer&;
    static auto getUniformBufferById(C::ID uBufferID)    -> const UniformBuffer&;
    static auto getUniformBuffer(const C::Tag& tag) -> UniformBuffer;
    


	GLuint getBlockUniformLocation(const C::Tag& uBlock, const C::Tag& uniform);
    static auto updateUniformBlock(C::ID uBufferID, C::ID uniformLocation);
    //void ShaderProgram::bindUniformBlockToAll(GLuint blockIndex);
};

}
