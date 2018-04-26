#include <overkill/Texture.hpp>
#include <fstream>

#include <PMS/logger.h>

namespace overkill 
{


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
