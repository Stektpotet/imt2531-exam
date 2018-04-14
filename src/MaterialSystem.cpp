#include <overkill/MaterialSystem.hpp>

namespace overkill  
{


std::vector<Material> MaterialSystem::m_materials;
std::unordered_map<C::Tag, C::ID> MaterialSystem::m_mapMaterialID;


auto MaterialSystem::getIdByTag(const C::Tag& tag) -> C::ID
{
    return m_mapMaterialID[tag];
}

auto MaterialSystem::getByTag(const C::Tag& tag) -> const Material&
{
    return MaterialSystem::m_materials[m_mapMaterialID[tag]];
}

auto MaterialSystem::getById(C::ID materialID) -> const Material&
{
    return MaterialSystem::m_materials[materialID];
}


void MaterialSystem::load()
{
    Material material;

    material.maps.push_back(
        UniformTexture
        {
            "mainTex",
            TextureSystem::copyByTag("BrickWall")
        }
    );
    material.maps.push_back(
        UniformTexture
        {
            "specularTex",
            TextureSystem::copyByTag("BrickWall_nrm")        
        }
    );
    material.floats.push_back(
        { "test", 0.5f }
    );

    MaterialSystem::m_mapMaterialID["brickwall"] = MaterialSystem::m_materials.size();
    MaterialSystem::m_materials.push_back(material);
}


}