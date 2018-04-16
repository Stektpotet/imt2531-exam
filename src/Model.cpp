#include <overkill/Model.hpp>


namespace overkill 
{

Mesh::Mesh(const std::vector<GLuint>& indices, const int shaderID, const int materialID)
{
    m_shaderID = shaderID;
    m_materialID = materialID;
    m_ebo = ElementBuffer(indices.data(), indices.size());
}

Model::Model(std::vector<Vertex> vertices) 
{
    m_vbo = VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));
    
    auto vbufLayout = VertexBufferAttribLayout();
    vbufLayout.push(3, GL_FLOAT);                       //position;
    vbufLayout.push(4, GL_INT_2_10_10_10_REV);                //normal
    vbufLayout.push(2, GL_SHORT);                       //uv
    vbufLayout.push(4, GL_UNSIGNED_BYTE, GL_TRUE);      //color;
    m_vao.addBuffer(m_vbo, vbufLayout);
}

void Model::pushMesh(const std::vector<GLuint> indices, const int shaderID, const int materialID)
{
    m_meshes.emplace_back( Mesh(indices, shaderID, materialID) );
}


}