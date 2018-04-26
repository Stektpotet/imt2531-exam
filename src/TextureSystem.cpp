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

void TextureSystem::push(const C::Tag tag, const std::string& filepath) 
{

    Texture texture;
    auto err = TextureSystem::makeTexture(filepath, &texture);
    if (err) {
        LOG_WARN("TextureSystem::makeTexture: failed, using default texture....");
        TextureSystem::m_mapTextureID[tag] = 0;
        return;
    }

    TextureSystem::m_mapTextureID[tag] = TextureSystem::m_textures.size();
    TextureSystem::m_textures.emplace_back( texture );
}

void TextureSystem::load()
{   
    // TODO: Load these from the file system somehow
    std::vector<FileEvent> fevents = Watcher::popEvents("discovered", "textures");
    
    TextureSystem::push("_default", "assets/textures/_default.jpg");
    LOG_DEBUG("Texture from file: assets/textures/_default.jpg");

    for (const auto e : fevents) {

        if (e.tag == "_default") continue;

        const auto filepath = C::TexturesFolder + ("/" + e.tag) + "." + e.extension;
        LOG_DEBUG("Texture from file: %s", filepath.data());
        TextureSystem::push(e.tag, filepath);

    }
}


auto TextureSystem::makeTexture(const std::string& filepath, Texture* outTexture) -> C::Err
{
    Texture texture;
    GLubyte* localBuffer;

    localBuffer = SOIL_load_image(filepath.c_str(), &texture.width, &texture.height, &texture.channels, SOIL_LOAD_AUTO);
    if (!localBuffer)
    {
        LOG_WARN("failed to load \"%s\"\n", filepath.c_str());
        return 1;
    }
    GLCall(glGenTextures(1, &texture.id));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture.id));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

#ifndef __APPLE__  // @NOTE Not supported on [macos 10.13, openGL 4.1]
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP)); 
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));
#endif


    glTexImage2D(GL_TEXTURE_2D,  
                 0, 
                 GL_RGB8, 
                 texture.width, 
                 texture.height, 
                 0, 
                 GL_RGB, 
                 GL_UNSIGNED_BYTE, 
                 localBuffer);

    auto err = GLLogFirstError();
    if (err) {
        LOG_WARN("glTexImage2D: Failed to buffer texture to GPU");
        texture.clean();
        return 1;
    }
    
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    SOIL_free_image_data(localBuffer);

    *outTexture = texture;
    return 0;
}

}
