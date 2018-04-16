#pragma once

#include <overkill/gl_caller.hpp>
#include <overkill/elementBuffer.hpp>
#include <overkill/vertexBuffer.hpp>
#include <overkill/vertexArray.hpp>
#include <overkill/renderer.hpp>


#include <vector>

namespace overkill 
{

struct Vertex
{
    GLfloat x,y,z;

    // @note normalized integers (decimal floating point number): 
    // @ref  https://www.khronos.org/opengl/wiki/Normalized_Integer
    // @ref  https://stackoverflow.com/questions/14036892/using-gl-int-2-10-10-10-rev-in-glvertexattribpointer
    GLint  normal;
    GLshort  u,v;
    //GLhalf  uv_2[2];
    //GLhalf  uv_3[2];
    // etc.
    GLubyte  r,g,b,a;
};

struct Triangle 
{
    int a, b, c;
};

struct Mesh
{
    Mesh(const std::vector<GLuint>& indices, const int shaderID, const int materialID);

    ElementBuffer m_ebo;
    int m_shaderID;
    int m_materialID;
};


struct Model 
{
    Model(std::vector<Vertex> vertices);
    void pushMesh(const std::vector<GLuint> indices, const int shaderID, const int materialID);

    VertexArray       m_vao;
    VertexBuffer      m_vbo;
    std::vector<Mesh> m_meshes;
};

}