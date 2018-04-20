#pragma once

#include <overkill/gl_caller.hpp>
#include <overkill/ShaderProgram.hpp>
#include <overkill/Config.hpp>
#include <vector>

namespace overkill
{
class UniformBuffer
{
private:
    std::vector<C::Tag> uniformBlocks; //each buffer knows which blocks it's holding
    GLuint m_id;
    C::Tag m_name;
public:

    UniformBuffer(const char *name, const void * data, const GLuint size, const GLenum drawMode) : m_name(name)
    {
        GLCall(glGenBuffers(1, &m_id));
        bind();
        GLCall(glBufferData(GL_UNIFORM_BUFFER, size, data, drawMode));
    }
   
    explicit operator C::Tag() const;
    explicit operator GLuint() const;

    void clean();

    void bind() const;
    void unbind() const;
    //void update()
   
};

//Streamed won't be neccesary i think...

}


//Shader definition
//ShaderSystem onLoad() -> Shader program introspection -> list of uniform block names
//Try lookup of uBlockName in ShaderSystem::m_uBlocks
