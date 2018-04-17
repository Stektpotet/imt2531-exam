#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <overkill/Config.hpp>
#include <overkill/Texture.hpp>
#include <overkill/Material.hpp>
#include <overkill/TextureSystem.hpp>
#include <overkill/Model.hpp>

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
    

public:

    // <summary> Load all model data onto GPU memory. 
    //           Keep model metadata on CPU </summary>
    static void load();
    static void reload();
    static auto getIdByTag(const C::Tag& materialTag) -> C::ID;
    static auto getByTag(const C::Tag& materialTag) -> const Material&;
    static auto getById(const C::ID materialID) -> const Material&;
    static void bindOnUpdate(const C::Tag& materialTag, C::ID modelID, C::ID meshID, OnUpdate onUpdate);
    static void unbindAll();
};

}


