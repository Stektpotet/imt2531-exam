#include "renderer.hpp"



void Renderer::clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

//void Renderer::draw(const VertexArray& va, const ElementBuffer& eb, const ShaderProgram& shader) const
void Renderer::draw(const VertexArray& va, const ElementBuffer& eb, const GLuint shader) const
{
    va.bind();
    eb.bind();
    GLCall(glUseProgram(shader));
    GLCall(glDrawElements(GL_TRIANGLES, eb.count(), GL_UNSIGNED_INT, nullptr));
    //
    // UNBIND [optional]... discuss
}

void EdgeRenderer::drawEdged(const VertexArray & va, const ElementBuffer & eb, const GLuint shader, const GLuint edgeShader) const
{
    va.bind();
    eb.bind();
    GLCall(glUseProgram(shader));
    GLCall(glDrawElements(GL_TRIANGLES, eb.count(), GL_UNSIGNED_INT, nullptr));

    GLCall(glUseProgram(edgeShader));
    GLCall(glLineWidth(4.0f))
    GLCall(glDrawElements(GL_LINE_STRIP, eb.count(), GL_UNSIGNED_INT, nullptr));
}
