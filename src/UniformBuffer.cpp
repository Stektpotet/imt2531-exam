#include <overkill/UniformBuffer.hpp>

namespace overkill
{
//lightblock
//mvpblock
//cam pos 

	UniformBuffer::UniformBuffer(const char * name, const UniformBufferLayout & layout, const GLenum drawMode)
		: m_name(name)
		, m_blockLayout(layout)
	{
		GLCall(glGenBuffers(1, &m_id));
		bind();
		GLCall(glBufferData(GL_UNIFORM_BUFFER, m_blockLayout.size(), nullptr, drawMode));
		unbind();
	}

	UniformBuffer::operator C::Tag() const
{
    return m_name;
}

UniformBuffer::operator GLuint() const
{
    return m_id;
}

void UniformBuffer::clean()
{
    GLCall(glDeleteBuffers(1, &m_id));
}

void UniformBuffer::bind() const
{
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_id));
}

void UniformBuffer::unbind() const
{
    GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

GLuint UniformBuffer::getUniformIndex(const C::Tag& name, GLuint blockInstance/*= 0*/) const
{
	return m_blockLayout.indexOfUniform(name, blockInstance);
}

void UniformBuffer::update(const C::ID index, GLsizeiptr size, const void * data)
{
	bind();
	glBufferSubData(GL_UNIFORM_BUFFER, index, size, data);
}

}