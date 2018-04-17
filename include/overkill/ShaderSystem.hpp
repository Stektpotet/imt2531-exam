#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <overkill/Config.hpp>
#include <overkill/ShaderProgram.hpp>

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
    static std::vector<ShaderProgram> m_shaderPrograms;
    static std::unordered_map<C::Tag, C::ID> m_mapShaderProgramID;
    static std::vector<UpdateCallback> m_updateCallbacks;
    static void push(const C::Tag&& tag, const char* path);

public:
    // <summary> Load all shader data onto GPU memory. 
    //           Keep shader metadata on CPU </summary>
    static void load();
    static void reload();
    static auto getIdByTag(const C::Tag& shaderTag) -> C::ID;
    static auto getByTag(const C::Tag& shaderTag) -> const ShaderProgram&;
    static auto getById(const C::ID shaderProgramID ) -> const ShaderProgram&;
    static auto copyByTag(const C::Tag& tag) -> ShaderProgram;
    static auto copyById(C::ID shaderProgramID) -> ShaderProgram;
    static void bindOnUpdate(const C::Tag& shaderTag, C::ID modelID, C::ID meshID, OnUpdate onUpdate);
};

}
