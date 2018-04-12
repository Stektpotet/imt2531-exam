#pragma once

#include <vector>
#include <overkill/gl_caller.hpp>

struct Uniform
{
    GLenum type;
};

class Material
{
private:
    std::vector<Uniform> properties;

public:

    //template<GLenum type>
    void push(GLenum type)
    {
        properties.push_back({type});
    }

    //template<>
    //void push<GL_SAMPLER_2D>()
    //{
    //    properties.push_back({ GL_SAMPLER_2D });
    //}
};
