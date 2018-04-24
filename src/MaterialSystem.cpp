
#include <overkill/MaterialSystem.hpp>

namespace overkill  
{

std::vector<Material>             MaterialSystem::m_materials;
std::unordered_map<C::Tag, C::ID> MaterialSystem::m_mapMaterialID;
std::vector<MaterialSystem::UpdateCallback> MaterialSystem::m_updateCallbacks;


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
    Watcher::pollEvents();
    MaterialSystem::load();

    for(auto onUpdate : MaterialSystem::m_updateCallbacks)
    {
        auto materialID =  MaterialSystem::getIdByTag(onUpdate.tag);
        onUpdate.callback(materialID, onUpdate.modelID, onUpdate.meshID);
    }
}

void MaterialSystem::push(const C::Tag tag, const std::string& filepath) 
{
    MaterialSystem::m_mapMaterialID[tag] = MaterialSystem::m_materials.size();
    MaterialSystem::m_materials.emplace_back(Material(filepath));
}

void MaterialSystem::load()
{
    std::vector<FileEvent> fevents = Watcher::popEvents("discovered", "materials");

    for(const auto e : fevents) {

        const auto filepath = C::MaterialsFolder + ("/" + e.tag) + "." + e.extension;
        LOG_DEBUG("Material from file: %s", filepath.data());
        MaterialSystem::push(e.tag, filepath);
    }
}

void MaterialSystem::bindOnUpdate(const C::Tag& materialTag, C::ID modelID, C::ID meshID, OnUpdate onUpdate)
{
    MaterialSystem::m_updateCallbacks.emplace_back(
        UpdateCallback{ 
            materialTag,
            modelID,
            meshID, 
            onUpdate 
        }
    );
}

void MaterialSystem::unbindAll()
{
    MaterialSystem::m_updateCallbacks.clear();
}


}
