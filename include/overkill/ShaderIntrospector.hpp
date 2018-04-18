#pragma once

#include <overkill/gl_caller.hpp>
#include <string>

namespace overkill
{
class ShaderIntrospector
{
    static GLuint UniformBlockIndex(const GLuint program, const std::string& name);
};

}