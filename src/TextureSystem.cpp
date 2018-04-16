#include <overkill/TextureSystem.hpp>
 #include <PMS/logger.h>
namespace overkill 
{

std::vector<Texture> TextureSystem::m_textures;
std::unordered_map<C::Tag, C::ID> TextureSystem::m_mapTextureID;


auto TextureSystem::getIdByTag(const C::Tag& tag) -> C::ID
{
    return m_mapTextureID[tag];
}

auto TextureSystem::getByTag(const C::Tag& tag) -> const Texture&
{
    return TextureSystem::m_textures[m_mapTextureID[tag]];
}

auto TextureSystem::getById(C::ID textureID) -> const Texture&
{
    return TextureSystem::m_textures[textureID];
}

auto TextureSystem::copyByTag(const C::Tag& tag) -> Texture
{
    return TextureSystem::m_textures[m_mapTextureID[tag]];
}

void TextureSystem::push(const C::Tag tag, const std::string&& filepath) 
{
    TextureSystem::m_mapTextureID[tag] = TextureSystem::m_textures.size();
    TextureSystem::m_textures.emplace_back(Texture(filepath));
}

void TextureSystem::load() 
{
    // TODO: Load these from the file system somehow
    std::vector<std::string> tags = {
        "checkers",
        "brickwall",
        "brickwall-nrm",
    };

    for (const auto tag : tags) {

        const auto filepath = C::TexturesFolder + tag + C::TexturesExtension;

        LOG_DEBUG("Texture from file: %s", filepath.data());
        TextureSystem::push(tag, C::TexturesFolder + tag + C::TexturesExtension);
    }
}


}
