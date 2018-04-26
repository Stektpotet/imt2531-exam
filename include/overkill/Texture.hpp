#pragma once

#include <SOIL.h>
#include <overkill/gl_caller.hpp>

namespace overkill 
{

class Texture
{
    public:
        GLuint      id;
        GLsizei     width, height, channels;

        Texture() = default;
		
		void clean();

        void bind(GLuint slot = 0) const;
        void unbind() const;

        inline GLsizei getWidth() const { return width; }
        inline GLsizei getHeight() const { return height; }
};

}
