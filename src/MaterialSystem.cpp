
#include <overkill/MaterialSystem.hpp>

namespace overkill  
{

std::vector<Material>             MaterialSystem::m_materials;
std::unordered_map<C::Tag, C::ID> MaterialSystem::m_mapMaterialID;
std::vector<UpdateCallback>       MaterialSystem::m_updateCallbacks;


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
    MaterialSystem::load();

    for(auto onUpdate : MaterialSystem::m_updateCallbacks)
    {
        auto materialID =  MaterialSystem::getIdByTag(onUpdate.tag);
        onUpdate.callback(materialID, onUpdate.mesh);
    }
}

void MaterialSystem::load()
{
    // TODO: Load these from the file system somehow
    const std::vector<std::string> tags = {
        "brick",
    };

    for(const auto tag : tags) {
        const auto filepath = C::MaterialsFolder + tag + C::MaterialsExtension;

        LOG_DEBUG("Material from file: %s", filepath.data());
        MaterialSystem::m_mapMaterialID[tag] = MaterialSystem::m_materials.size();
        MaterialSystem::m_materials.emplace_back(Material(filepath));
    }
}

void MaterialSystem::bindOnUpdate(const C::Tag& materialTag, Mesh& mesh, OnUpdate onUpdate) 
{
    MaterialSystem::m_updateCallbacks.emplace_back(
        UpdateCallback{ 
            materialTag, 
            mesh, 
            onUpdate 
        }
    );
}


}