#include <overkill/TextureSystem.hpp>

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

void TextureSystem::push(const C::Tag tag, const char* path) 
{
    TextureSystem::m_mapTextureID[tag] = TextureSystem::m_textures.size();
    TextureSystem::m_textures.emplace_back(Texture(path));
}

void TextureSystem::load() 
{
    TextureSystem::push("BrickWall", "assets/textures/Checkers.jpg");
    TextureSystem::push("BrickWall_nrm", "assets/textures/BrickWall_nrm.jpg");
}


}