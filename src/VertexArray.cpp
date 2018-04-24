#include <overkill/VertexArray.hpp>
#include <overkill/gl_util.hpp>


namespace overkill 
{

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &id));
    GLCall(glBindVertexArray(id));
}

void VertexArray::clean()
{
    GLCall(glDeleteVertexArrays(1, &id));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferAttribLayout& layout)
{
    bind();
    vb.bind();
    GLuint i = 0;
    GLuint offset = 0;
    for (const auto& attrib : layout.getAttributes())
    {
        GLCall(glEnableVertexAttribArray(i));
        GLCall(
            glVertexAttribPointer(
                i,
                attrib.componentCount,
                attrib.type,
                attrib.normalized,
                layout.getStride(),
                (const void*)offset
            )
        );
        offset += attrib.size;
        i++;
    }

}

void VertexArray::bind() const
{
    GLCall(glBindVertexArray(id));
}

void VertexArray::unbind() const
{
    GLCall(glBindVertexArray(0));
}

}