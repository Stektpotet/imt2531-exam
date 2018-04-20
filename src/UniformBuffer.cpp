#include <overkill/UniformBuffer.hpp>

namespace overkill
{
//lightblock
//mvpblock
//cam pos 

UniformBuffer::operator C::Tag() const
{
    return m_name;
}

UniformBuffer::operator GLuint() const
{
    return m_id;
}
//
//UniformBuffer::Update(GLuint uniformLocation, GLuint size)
//{
// /*   bind();
//    GLCall(glBufferSubData(GL_UNIFORM_BUFFER, ));*/
//}


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


}