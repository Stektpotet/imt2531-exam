#pragma once

#include <overkill/gl_caller.hpp>
#include <overkill/ShaderProgram.hpp>
#include <overkill/Config.hpp>

namespace overkill
{
template<GLenum drawMode>
class UniformBuffer
{
private:
    GLuint m_id;
    C::Tag m_name;
public:

    UniformBuffer(const char *name, const void * data, GLuint size) : m_name(name)
    {
        GLCall(glGenBuffers(1, &m_id));
        bind();
        GLCall(glBufferData(GL_UNIFORM_BUFFER, size, data, drawMode));
    }
   /*
    explicit operator C::Tag() const;
    explicit operator GLuint() const;

    void clean();

    void bind() const;
    void unbind() const;*/
    
    explicit operator C::Tag() const
    {
        return m_name;
    }

    explicit operator GLuint() const
    {
        return m_id;
    }

    void clean()
    {
        GLCall(glDeleteBuffers(1, &m_id));
    }

    void bind() const
    {
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_id));
    }

    void unbind() const
    {
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
    }

};

using StaticUniformBuffer = UniformBuffer<GL_STATIC_DRAW>;
using DynamicUniformBuffer = UniformBuffer<GL_DYNAMIC_DRAW>;
//Streamed won't be neccesary i think...

}