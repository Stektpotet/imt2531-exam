#pragma once

#include <overkill/gl_caller.hpp>
#include <string>
#include <vector>

namespace overkill
{
class ShaderIntrospector
{
public:

    //UNIFORM BLOCKS
    static GLint  getActiveBlockCount(const GLuint program);
    static const  std::vector<GLint> getUniformBlockUniformIndices(const GLuint program, const GLuint uBlockIndex);
    static const  std::string getUnifromBlockName(const GLuint program, const GLint uBlockIndex);
    static GLuint getUniformBlockIndex(const GLuint program, const std::string& name);

    //UNIFROMS
    static GLint  getActiveUniformCount(const GLuint program);


    //ATTRIBUTES
    static GLint  getActiveAttribCount(const GLuint program);

};

}
