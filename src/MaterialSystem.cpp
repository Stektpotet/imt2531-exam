
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

    C::Err err;

    std::string materialString;
    err = Util::fileToString(filepath, &materialString);
    if (err) {
        LOG_WARN("Util::fileToString: Could not load material from file, using default material...");
        MaterialSystem::m_mapMaterialID[tag] = 0;
        return;
    }

    Material material;
    err = MaterialSystem::makeMaterial(materialString, &material);
    if (err) {
        LOG_WARN("Util::makeMaterial: failed parsing material string, using default material...")
        MaterialSystem::m_mapMaterialID[tag] = 0;
        return;
    }

    MaterialSystem::m_mapMaterialID[tag] = MaterialSystem::m_materials.size();
    MaterialSystem::m_materials.emplace_back(material);
}

void MaterialSystem::load()
{
    std::vector<FileEvent> fevents = Watcher::popEvents("discovered", "materials");

    MaterialSystem::push("_default", "assets/materials/_default.yml");
    LOG_DEBUG("Material from file: assets/materials/_default.yml");

    for(const auto e : fevents) {

        if (e.tag == "_default") continue;

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


auto MaterialSystem::makeMaterial(const std::string& materialString, Material* outMaterial) -> C::Err 
{
    auto p = Parser(materialString);

    Material material;

    // Iterating through maps from material file
    auto[mapcountkey, mapcount, err1] = p.nextKeyInteger();
    if (err1 == PARSE_ERROR)
        return 1;

    for (int i = 0; i < mapcount; ++i) 
    {
        auto[uniformtag, maptag, err2] = p.nextKeyString();
        if (err2 == PARSE_ERROR)
            return 1;

        material.m_unimaps.emplace_back(
            UniformTexture {
                uniformtag,
                TextureSystem::copyByTag(maptag)
            }
        );
    }

    // Iterating through values from material file
    auto[valueskey, valuescount, err3] = p.nextKeyInteger();
    if (err3 == PARSE_ERROR)
        return 1;
        
    for (int i = 0; i < valuescount; ++i) 
    {
        auto[uniformtag, value, err4] = p.nextKeyFloat();
        if (err4 == PARSE_ERROR)
            return 1;

        material.m_univalues.emplace_back(
            UniformFloat{ uniformtag, value }
        );
    }

    *outMaterial = material;
    return 0;
}

}
