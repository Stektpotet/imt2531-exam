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

    /// <summary>
    /// <param name="count">The number of components the attribute has in the shaderprogram</param> 
    /// <param name="type">The number of components the attribute has in the shaderprogram</param> 
    /// <param name="normalized">wether or not the values should be normalized</param> 
    /// </summary>

	template<GLenum type>
    void push(GLuint count, bool normalized = false)
    {
        m_attributes.push_back({ count, type, normalized });
		m_stride += count * GLTypeSize(type);
	}

	template<>
	void push<GL_INT_2_10_10_10_REV>(GLuint count, bool normalized)
	{
		m_attributes.push_back({ count, GL_INT_2_10_10_10_REV, normalized });
        m_stride += count; //TODO allow for attributes to contain more than one withouth striding errors
	}

    inline const std::vector<VertexBufferAttrib> getAttributes() const { return m_attributes; }
    inline GLuint getStride() const { return m_stride; }
};

}