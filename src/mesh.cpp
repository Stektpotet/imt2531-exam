#include <overkill/mesh.hpp>

//Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const int shaderID, const int materialID)
//{
//    m_vbo = VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));
//    
//    m_shaderID = shaderID;
//    m_materialID = materialID;
//
//    auto vbufLayout = VertexBufferAttribLayout();
//    vbufLayout.push(3, GL_FLOAT);                       //position;
//    vbufLayout.push(1, GL_UNSIGNED_INT);                //normal
//    vbufLayout.push(2, GL_SHORT);                       //uv
//    vbufLayout.push(4, GL_UNSIGNED_BYTE, GL_TRUE);      //color;
//    
//    m_vao.addBuffer(m_vbo, vbufLayout);
//
//    m_ebo = ElementBuffer(indices.data(), indices.size());
//}

void Mesh::DrawMePles(const Renderer& renderer)
{
    renderer.clear();
    ShaderProgram shader("assets/shaders/base.shader");
    renderer.draw(m_vao, m_ebo, shader);
}
//
Mesh::~Mesh()
{
}
//
//Mesh::Mesh(const VertexBuffer & vbo) : m_vbo(vbo){}
