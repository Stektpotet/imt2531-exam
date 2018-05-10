
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


auto MaterialSystem::loadOBJ(const std::vector<tinyobj::material_t>& materials, 
                             const C::Tag& objTag) -> C::Err
{


    for(const auto& objMaterial: materials) 
    {
        Material overkillMaterial; 

        overkillMaterial.m_univectors.push_back(UniformVec3{ "ambient", glm::vec3{
              objMaterial.ambient[0],
              objMaterial.ambient[1],
              objMaterial.ambient[2]
        }});

        overkillMaterial.m_univectors.push_back(UniformVec3{ "diffuse", glm::vec3{
              objMaterial.diffuse[0],
              objMaterial.diffuse[1],
              objMaterial.diffuse[2]
        }});

        overkillMaterial.m_univectors.push_back(UniformVec3{ "specular", glm::vec3{
              objMaterial.specular[0],
              objMaterial.specular[1],
              objMaterial.specular[2]
        }});

        overkillMaterial.m_univectors.push_back(UniformVec3{ "transmittance", glm::vec3{
              objMaterial.transmittance[0],
              objMaterial.transmittance[1],
              objMaterial.transmittance[2]
        }});

        overkillMaterial.m_univectors.push_back(UniformVec3{ "emission", glm::vec3{
              objMaterial.emission[0],
              objMaterial.emission[1],
              objMaterial.emission[2]
        }});

        overkillMaterial.m_univalues.push_back(UniformFloat {"shininess", objMaterial.shininess});
        overkillMaterial.m_univalues.push_back(UniformFloat {"ior", objMaterial.ior});  // Dunno what this is suppposed to be
        overkillMaterial.m_univalues.push_back(UniformFloat {"dissolve", objMaterial.dissolve});
        overkillMaterial.m_univalues.push_back(UniformFloat {"illuminosity", static_cast<float>(objMaterial.illum) });


        auto resolveTexture = [](Material& overkillMaterial, 
                                const C::Tag& objTag,
                                 const std::string filename,
                                 const std::string type)
        {
            if (!filename.empty()) 
            {
                auto filenameOnly = filename.substr(0, filename.find('.'));
                auto textureTag = "obj/" + objTag + "/" + filenameOnly + "." + type;

                if (TextureSystem::getIdByTag(textureTag) != 0)
                {
                    overkillMaterial.m_unimaps.push_back(
                        UniformTexture{
                            "map_" + type,
                            TextureSystem::copyByTag(textureTag)
                        });
                }
            }
        };

        resolveTexture(overkillMaterial, objTag, objMaterial.ambient_texname, "ambient");
        resolveTexture(overkillMaterial, objTag, objMaterial.diffuse_texname, "diffuse");
        resolveTexture(overkillMaterial, objTag, objMaterial.specular_texname, "specular");
        resolveTexture(overkillMaterial, objTag, objMaterial.specular_highlight_texname, "specular_highlight");
        resolveTexture(overkillMaterial, objTag, objMaterial.bump_texname, "bump");
        resolveTexture(overkillMaterial, objTag, objMaterial.alpha_texname, "alpha");
        resolveTexture(overkillMaterial, objTag, objMaterial.displacement_texname, "displacement");


        overkillMaterial.m_tag = "obj/" + objTag + "/" + objMaterial.name;
        LOG_DEBUG("loading material: %s",overkillMaterial.m_tag.data());

        MaterialSystem::m_mapMaterialID[overkillMaterial.m_tag] = MaterialSystem::m_materials.size();
        MaterialSystem::m_materials.emplace_back(overkillMaterial);
    }

    return 0;
}


}
