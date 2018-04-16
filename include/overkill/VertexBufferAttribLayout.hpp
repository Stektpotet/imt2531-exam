#pragma once

#include <vector>
#include <overkill/gl_util.hpp>


namespace overkill 
{

struct VertexBufferAttrib
{
    GLuint      count;
    GLenum      type;
    GLboolean   normalized;
};
//(GLuint index,
// GLint size,
// GLenum type,
// GLboolean normalized,
// GLsizei stride,
// const void* pointer);
struct VertexBufferAttribLayout
{
private:
    std::vector<VertexBufferAttrib> m_attributes;
    GLuint m_stride;
public:
    VertexBufferAttribLayout() : m_stride(0) {}

    void push(GLuint count, GLenum type, bool normalized = false)
    {
        m_attributes.push_back({ count, type, normalized });
        if(type == GL_INT_2_10_10_10_REV)
            m_stride += GLTypeSize(type);
        else
            m_stride += count * GLTypeSize(type);
    }

    inline const std::vector<VertexBufferAttrib> getAttributes() const { return m_attributes; }
    inline GLuint getStride() const { return m_stride; }
};

}