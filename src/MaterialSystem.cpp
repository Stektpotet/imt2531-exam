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


void MaterialSystem::reload() 
{
    MaterialSystem::m_materials.clear();
}

void MaterialSystem::load()
{
    // TODO: Load these from the file system somehow
    std::vector<std::string> tags = {
        "brick",
    };

    for(const auto tag : tags) {
        const auto filepath = C::MaterialsFolder + tag + C::MaterialsExtension;

        LOG_DEBUG("Material from file: %s", filepath.data());
        MaterialSystem::m_mapMaterialID[tag] = MaterialSystem::m_materials.size();
        MaterialSystem::m_materials.emplace_back(Material(filepath));
    }
}


}