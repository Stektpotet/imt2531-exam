#include <overkill/Renderer.hpp>

namespace overkill
{

void Renderer::clear()
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

//void Renderer::draw(const VertexArray& va, const ElementBuffer& eb, const ShaderProgram& shader) const
void Renderer::draw(const VertexArray& va, const ElementBuffer& eb, const ShaderProgram& shader)
{
    va.bind();
    eb.bind();
    shader.bind();
    GLCall(glDrawElements(GL_TRIANGLES, eb.count(), GL_UNSIGNED_INT, nullptr));
    //
    // UNBIND [optional]... discuss
}

void Renderer::draw(const Model& model, glm::mat4 modelMatrix)
{

    model.m_vao.bind();
    
    for(auto& mesh: model.m_meshes) 
    {
        mesh.m_ebo.bind();
        mesh.m_shaderProgram.bind();
        GLint uniformModel = mesh.m_shaderProgram.getUniformLocation("model");
        GLCall(glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix)));
        GLCall(glDrawElements(GL_TRIANGLES, mesh.m_ebo.count(), GL_UNSIGNED_INT, nullptr));
    }

    //
    // UNBIND [optional]... discuss
}

void EdgeRenderer::drawEdged(const VertexArray & va, const ElementBuffer & eb, const ShaderProgram& shader, const ShaderProgram& edgeShader) const
{
    va.bind();
    eb.bind();
    shader.bind();

    GLCall(glDrawElements(GL_TRIANGLES, eb.count(), GL_UNSIGNED_INT, nullptr));

    edgeShader.bind();
    GLCall(glLineWidth(1.0f))
        GLCall(glDrawElements(GL_LINE_STRIP, eb.count(), GL_UNSIGNED_INT, nullptr));
}

}
