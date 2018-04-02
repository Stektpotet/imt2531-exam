#pragma once

#include <gl_caller.hpp>

#include <vertexBuffer.hpp>
#include <vertexBufferAttribLayout.hpp>

class VertexArray
{

private:
    GLuint id;
public:
    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer& vb, const VertexBufferAttribLayout& layout);

    void bind() const;
    void unbind() const;
};

