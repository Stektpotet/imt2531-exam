#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <tiny_obj_loader/tiny_obj_loader.h>

#include <overkill/Config.hpp>
#include <overkill/Texture.hpp>
#include <overkill/Watcher.hpp>
#include <PMS/logger.h>

namespace overkill
{

class C;

class TextureSystem 
{
private:
    static std::vector<Texture> m_textures;
    static std::unordered_map<C::Tag, C::ID> m_mapTextureID;
    static void push(const C::Tag tag, const std::string& filepath);

public:
    // <summary> Load all texture data onto GPU memory. 
    //           Keep model metadata on CPU </summary>
    static void load();
    static auto loadOBJ(const std::vector<tinyobj::material_t>& materials,
                        const std::string& objBasedir,
                        const C::Tag& baseTag) -> C::Err;
    static void clean();
    static auto getIdByTag(const C::Tag& textureTag) -> C::ID;
    static auto getByTag(const C::Tag& textureTag) -> const Texture&;
    static auto getById(const C::ID textureID) -> const Texture&;
    static auto copyByTag(const C::Tag& tag) -> Texture;
    static auto makeTexture(const std::string& filepath, Texture* outTexture) -> C::Err;    
};

}
