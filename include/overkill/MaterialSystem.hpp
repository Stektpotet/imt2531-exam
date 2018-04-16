#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <overkill/Config.hpp>
#include <overkill/texture.hpp>
#include <overkill/Material.hpp>
#include <overkill/TextureSystem.hpp>

namespace overkill
{

class MaterialSystem
{
private:
    static std::vector<Material> m_materials;
    static std::unordered_map<C::Tag, C::ID> m_mapMaterialID;

public:
    // <summary> Load all model data onto GPU memory. 
    //           Keep model metadata on CPU </summary>
    static void load();
    static void reload();
    static auto getIdByTag(const C::Tag& materialTag) -> C::ID;
    static auto getByTag(const C::Tag& materialTag) -> const Material&;
    static auto getById(const C::ID materialID) -> const Material&;
};

}
