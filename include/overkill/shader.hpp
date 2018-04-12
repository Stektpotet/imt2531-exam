#pragma once

#include <fstream>
#include <sstream>
#include <unordered_map>
#include <tuple>

#include <GL/glew.h>

#include <overkill/gl_caller.hpp>
#include <overkill/material.hpp>

// using ShaderSource = std::tuple<std::string, std::string, std::string>;

struct ShaderSource
{
    std::string vertex;
    std::string fragment;
    std::string geometry;
};
class Shader
{
    struct Uniform
    {
        GLint location;
        GLenum type; // type of the variable (float, vec3 or mat4, etc)
        GLchar name[16]; // variable name in GLSL
    };
private:
    std::unordered_map<std::string, GLint> uniforms;

    void construct(const std::string& vert, const std::string& frag, const std::string& geom);

    GLuint id;
public:

    explicit operator GLuint() const;

    Shader(const std::string& vert, const std::string& frag, const std::string& geom);
    Shader(const std::string& filePath);
    ~Shader();

    void bind(const Material& mat) const; // const; - can't be const as uniform cache might be updated
    void unbind() const;


    GLint getUniformLocation(const std::string& name) const;
};

static ShaderSource ParseProgram(const std::string& file);
static GLuint CompileShader(GLuint type, const std::string& source);
