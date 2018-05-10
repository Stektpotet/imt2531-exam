#include <overkill/VertexBuffer.hpp>


namespace overkill 
{

VertexBuffer::VertexBuffer(const void * data, GLuint size)
{
    GLCall(glGenBuffers(1, &m_rendererID));
    bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::clean()
{
    unbind();
    GLCall(glDeleteBuffers(1, &m_rendererID));
}

void VertexBuffer::update(GLintptr start, GLsizeiptr size, const GLvoid * data)
{
    bind();
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, start, size, data));
}

void VertexBuffer::bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
}

void VertexBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

}
