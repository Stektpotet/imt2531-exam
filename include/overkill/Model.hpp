#pragma once

#include <overkill/gl_caller.hpp>
#include <overkill/ElementBuffer.hpp>
#include <overkill/VertexBuffer.hpp>
#include <overkill/VertexArray.hpp>
#include <overkill/Config.hpp>
#include <overkill/ShaderProgram.hpp>

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
    C::Tag        m_tag;  
    ElementBuffer m_ebo;
    C::ID         m_materialID;
    ShaderProgram m_shaderProgram;
};


struct Model 
{
    C::Tag            m_tag;  
    VertexArray       m_vao;
    VertexBuffer      m_vbo;
    std::vector<Mesh> m_meshes;
};

}
