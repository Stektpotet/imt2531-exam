#pragma once

#include <SOIL.h>
#include <overkill/gl_caller.hpp>


namespace overkill 
{

class Texture
{
    private:
        GLuint      id;
        std::string filePath;
        GLubyte*    localBuffer;
        GLsizei     width, height, channels;
    public:
        Texture(const std::string& filePath);
		
		
		void clean();

        void bind(GLuint slot = 0) const;
        void unbind() const;

        inline GLsizei getWidth() const { return width; }
        inline GLsizei getHeight() const { return height; }
};

}