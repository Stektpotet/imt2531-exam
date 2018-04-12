#pragma once

#include <SOIL.h>
#include <overkill/gl_caller.hpp>

class Texture
{
    private:
        GLuint      id;
        std::string filePath;
        GLubyte*    localBuffer;
        GLsizei     width, height, channels;
    public:
        Texture(const std::string& filePath);
        ~Texture();

        void bind(GLuint slot = 0) const;
        void unbind() const;

        inline GLsizei getWidth() const { return width; }
        inline GLsizei getHeight() const { return height; }
};
