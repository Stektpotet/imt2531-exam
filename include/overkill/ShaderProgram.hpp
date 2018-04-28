#pragma once

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <array>
#include <GL/glew.h>

#include <overkill/gl_caller.hpp>
#include <overkill/gl_util.hpp>
#include <overkill/Texture.hpp>
#include <overkill/Material.hpp>
#include <overkill/ShaderIntrospector.hpp>

namespace overkill 
{

// using ShaderSource = std::tuple<std::string, std::string, std::string>;

struct ShaderSource
{
    std::string vertex;
    std::string fragment;
    std::string geometry;
};

class ShaderProgram
{
public:
    struct Uniform
    {
        GLint location;
        GLenum type; // type of the variable (float, vec3 or mat4, etc)
        GLchar name[16]; // variable name in GLSL
    };
    
    enum PropertyType
    {
        Enable      = 0,  //glEnable/Disable
        BlendFunc   = 1,  //glBlendFunc
        CullFace    = 2   //glCullFace
    };
    std::array<std::vector<GLenum>, 3> drawProperties;

    std::unordered_map<std::string, GLint> uniforms;
    std::unordered_map<std::string, GLint> uniformBlocks;

    C::Tag m_tag;

    GLuint id;

    explicit operator GLuint() const;

    ShaderProgram() = default;
    ShaderProgram(const ShaderProgram&) = default;
    
    
    void clean();

    void setMaterial(const Material& mat) const;
    void bind() const;
    void unbind() const;

    GLint getUniformLocation(const std::string& name) const;
    
};

// @note unused functions

}