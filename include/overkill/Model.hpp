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
// @note normalized integers (decimal floating point number): 
// @ref  https://www.khronos.org/opengl/wiki/Normalized_Integer
// @ref  https://stackoverflow.com/questions/14036892/using-gl-int-2-10-10-10-rev-in-glvertexattribpointer
// @ref  https://www.khronos.org/opengl/wiki/Vertex_Specification_Best_Practices
struct Vertex
{
    GLfloat  x,y,z;
    GLint    n;
    GLushort u=0,v=0;
    GLubyte  r=255,g=255,b=255,a=255;
};

struct Triangle 
{
    GLuint a, b, c;
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
