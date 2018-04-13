#pragma once

#include <overkill/gl_caller.hpp>
#include <overkill/elementBuffer.hpp>
#include <overkill/vertexBuffer.hpp>
#include <overkill/vertexArray.hpp>
#include <overkill/renderer.hpp>


#include <vector>

struct Vertex
{
    GLfloat position[3];

    // @note normalized integers (decimal floating point number): 
    // @ref  https://www.khronos.org/opengl/wiki/Normalized_Integer
    // @ref  https://stackoverflow.com/questions/14036892/using-gl-int-2-10-10-10-rev-in-glvertexattribpointer
    GLuint  normal;
    GLshort  uv_1[2];
    //GLhalf  uv_2[2];
    //GLhalf  uv_3[2];
    // etc.
    GLubyte  color[4];
};

//@NOTE:    It would make sense to move buffers(VBO/VAO) outside of the mesh to fully 
//          utilize the buffers (and thereby optimize performance.. I guess?)
//          @date 13.04.2018
class Mesh
{
public:
    //Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const int shaderID, const int materialID = 0);
    ~Mesh();
    //Mesh(const VertexBuffer& vbo);
    void DrawMePles(const Renderer& renderer);

    //std::vector<Vertex> m_vertices;
    //std::vector<GLuint> m_indices;
public:
    VertexBuffer m_vbo;
    VertexArray m_vao;
    ElementBuffer m_ebo;
    int m_shaderID;
    int m_materialID;
};
