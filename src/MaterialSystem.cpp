
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
        LOG_WARN("Util::fileToString: Could not load material from file %s", filepath.data());
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
    material.m_tag = tag;
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
    auto parse = Parser(materialString);

    Material material;

    // Iterating through maps from material file
    int mapCount;
    if (auto[key, mapCount_, err] = parse.keyInteger("maps"); err) {
        return 1;
    } else {
        mapCount = mapCount_;
    }


    for (int i = 0; i < mapCount; ++i) 
    {
        if (auto[uniformTag, textureTag, err] = parse.keyString(); err) {
            return 1;            
        } else {

            material.m_unimaps.emplace_back(
                UniformTexture {
                    std::string(uniformTag),
                    TextureSystem::copyByTag(std::string(textureTag))
                }
            );
        }
    }

    // Iterating through values from material file
    int valuesCount;
    if(auto[key, valuesCount_, err] = parse.keyInteger("values"); err) {
        return 1;
    } else {
        valuesCount = valuesCount_;
    }
        
    for (int i = 0; i < valuesCount; ++i) 
    {
        if (auto[uniformTag, value, err] = parse.keyFloat(); err) {
            return 1;
        } else {
            material.m_univalues.emplace_back(
                UniformFloat{ std::string(uniformTag), value }
            );
        }
    }

    *outMaterial = material;
    return 0;
}

}
