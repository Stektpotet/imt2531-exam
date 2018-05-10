#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <tiny_obj_loader/tiny_obj_loader.h>

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
    static auto loadOBJ(const tinyobj::attrib_t&                attributes,
                        const std::vector<tinyobj::shape_t>&    shapes,
                        const std::vector<tinyobj::material_t>& materials,
                        const C::Tag&                           objTag)->C::Err;

    static void reload();
    static void clean();
    static auto getIdByTag(const C::Tag& modeltag) -> C::ID;
    static auto getByTag(const C::Tag& tag) -> Model&;
    static auto getById(const C::ID modelID) -> Model&;

    static auto makeModel(const C::Tag& tag, const std::string& modelString, Model* outModel) -> C::Err;
    static void push(const std::string& tag, const std::string& filepath);

};

}
