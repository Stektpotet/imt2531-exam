#include <overkill/texture.hpp>
#include <fstream>


namespace overkill 
{

Texture::Texture(const std::string& filePath)
    :id(0), filePath(filePath), localBuffer(nullptr),
    width(0), height(0), channels(0)
{
    localBuffer = SOIL_load_image(filePath.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
    if (!localBuffer)
    {
        printf("failed to load \"%s\"\n", filePath.c_str());
    }
    GLCall(glGenTextures(1, &id));
    GLCall(glBindTexture(GL_TEXTURE_2D, id));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

#ifndef __APPLE__  // @NOTE Not supported on [macos 10.13, openGL 4.1]
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP)); 
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));
#endif

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, localBuffer));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    if (localBuffer)
    {
        SOIL_free_image_data(localBuffer);
    }
}
void Texture::clean()
{
    GLCall(glDeleteTextures(1, &id));
}

void Texture::bind(GLuint slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, id));
}

void Texture::unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

}