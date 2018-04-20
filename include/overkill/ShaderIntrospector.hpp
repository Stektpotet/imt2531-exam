#pragma once

#include <overkill/gl_caller.hpp>
#include <string>
#include <vector>

namespace overkill
{
class ShaderIntrospector
{
public:
    static GLint  getActiveBlockCount(const GLuint program);
    static const  std::vector<GLint> getUniformBlockUniformIndices(const GLuint program, const GLuint uBlockIndex);
    static const  std::string getUnifromBlockName(const GLuint program, const GLint uBlockIndex);
    static GLuint getUniformBlockIndex(const GLuint program, const std::string& name);
    static GLuint getUniformBufferMaxSize(const GLuint program);
    static GLint  getUniformBlockBinding(const GLuint program, GLuint uniformBlockIndex);
};

}

/*

map:
    tag:
        program: blockIndex
        program: blockIndex


*/