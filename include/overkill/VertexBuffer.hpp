#pragma once

#include <overkill/gl_caller.hpp>


namespace overkill 
{

class VertexBuffer
{
private:
    GLuint m_rendererID;
public:
    VertexBuffer() = default;
    VertexBuffer(const void* data, GLuint size);
    
    
    void clean();

    void update(GLintptr start, GLuint size, const void * data);
    void bind() const;
    void unbind() const;
};

}