#pragma once

#include <gl_caller.hpp>

class VertexBuffer
{
private:
    unsigned int m_rendererID;
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void update(GLintptr start, GLuint size, const void * data);
    void bind() const;
    void unbind() const;
};