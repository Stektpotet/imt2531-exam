#pragma once

#include <vector>
#include <overkill/gl_util.hpp>


namespace overkill 
{

struct VertexBufferAttrib
{
    GLuint      size;
    GLuint      componentCount;
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

    /// <summary>
    /// <param name="count">The number of components the attribute has in the shaderprogram</param> 
    /// <param name="type">The number of components the attribute has in the shaderprogram</param> 
    /// <param name="normalized">wether or not the values should be normalized</param> 
    /// </summary>

	template<GLenum type>
    void push(GLuint count, bool normalized = false)
    {
        auto size = count * GLTypeSize(type);
        m_attributes.push_back({ size, count, type, normalized });
		m_stride += size;
	}

    template<GLenum type>
    void pushPacked(GLuint componentCount, GLuint count = 1, bool normalized = false)
    {
        auto size = count * GLTypeSize(type);
        m_attributes.push_back({ size, componentCount, type, normalized });
        m_stride += size;
    }

    inline const std::vector<VertexBufferAttrib> getAttributes() const { return m_attributes; }
    inline GLuint getStride() const { return m_stride; }
};

}
