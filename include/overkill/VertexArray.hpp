#pragma once

#include <overkill/gl_caller.hpp>

#include <overkill/VertexBuffer.hpp>
#include <overkill/VertexBufferAttribLayout.hpp>


namespace overkill 
{

class VertexArray
{

private:
    GLuint id;
public:
    VertexArray();
    
    
    void clean();

    void addBuffer(const VertexBuffer& vb, const VertexBufferAttribLayout& layout);

    void bind() const;
    void unbind() const;
};

}