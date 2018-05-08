#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <tiny_obj_loader/tiny_obj_loader.h>

#include <overkill/Config.hpp>
#include <overkill/Texture.hpp>
#include <overkill/Material.hpp>
#include <overkill/TextureSystem.hpp>
#include <overkill/Model.hpp>
#include <overkill/Parser.hpp>



namespace overkill
{

class MaterialSystem
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
    static std::vector<Material> m_materials;
    static std::unordered_map<C::Tag, C::ID> m_mapMaterialID;
    static std::vector<UpdateCallback> m_updateCallbacks;
    
    static void push(const C::Tag tag, const std::string& filepath);
    static auto makeMaterial(const std::string& materialString, Material* outMaterial) -> C::Err;


public:

    // <summary> Load all model data onto GPU memory. 
    //           Keep model metadata on CPU </summary>
    static void load();
    static auto loadOBJ(const std::vector<tinyobj::material_t>& materials,
                        const C::Tag& baseTag) -> C::Err;
    static void reload();
    static auto getIdByTag(const C::Tag& materialTag) -> C::ID;
    static auto getByTag(const C::Tag& materialTag) -> const Material&;
    static auto getById(const C::ID materialID) -> const Material&;
    static void bindOnUpdate(const C::Tag& materialTag, C::ID modelID, C::ID meshID, OnUpdate onUpdate);
    static void unbindAll();
};

}


