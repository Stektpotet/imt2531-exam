#include <overkill/vertexArray.hpp>
#include <overkill/gl_util.hpp>

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &id));
    GLCall(glBindVertexArray(id));
}

VertexArray::~VertexArray()
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
                attrib.count,
                attrib.type,
                attrib.normalized,
                layout.getStride(),
                (const void*)offset
            )
        );
        offset += attrib.count * GLTypeSize(attrib.type);
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
