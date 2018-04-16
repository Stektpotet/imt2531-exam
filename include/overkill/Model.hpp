#pragma once

#include <overkill/gl_caller.hpp>
#include <overkill/ElementBuffer.hpp>
#include <overkill/VertexBuffer.hpp>
#include <overkill/VertexArray.hpp>
#include <overkill/Renderer.hpp>
#include <overkill/ShaderSystem.hpp>


#include <vector>

namespace overkill 
{

struct Vertex
{
    GLfloat x,y,z;

    // @note normalized integers (decimal floating point number): 
    // @ref  https://www.khronos.org/opengl/wiki/Normalized_Integer
    // @ref  https://stackoverflow.com/questions/14036892/using-gl-int-2-10-10-10-rev-in-glvertexattribpointer
    GLuint  normal;
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
    Mesh()=default;
    Mesh(const std::vector<GLuint>& indices, const int materialID, const ShaderProgram shaderProgram);

    ElementBuffer m_ebo;
    ShaderProgram m_shaderProgram;
    int           m_materialID;
};


struct Model 
{
    Model(std::vector<Vertex> vertices);
    void pushMesh(const Mesh mesh);

    VertexArray       m_vao;
    VertexBuffer      m_vbo;
    std::vector<Mesh> m_meshes;
};

}