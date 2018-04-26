#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <overkill/Config.hpp>
#include <overkill/Model.hpp>
#include <overkill/ShaderSystem.hpp>
#include <overkill/ElementBuffer.hpp>
#include <overkill/VertexBuffer.hpp>
#include <overkill/VertexArray.hpp>
#include <overkill/MaterialSystem.hpp>
#include <overkill/Parser.hpp>
#include <overkill/Util.hpp>

namespace overkill
{

class ModelSystem 
{
private:
    static std::vector<Model> m_models;
    static std::unordered_map<std::string, C::ID> m_mapModelID;

public:
    // <summary> Load all model data onto GPU memory. 
    //           Keep model metadata on CPU </summary>
    static void load();
    static void reload();
    static auto getIdByTag(const C::Tag& modeltag) -> C::ID;
    static auto getByTag(const C::Tag& tag) -> Model&;
    static auto getById(const C::ID modelID) -> Model&;
};

}
