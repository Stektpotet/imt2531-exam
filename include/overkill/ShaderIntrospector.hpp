#pragma once

#include <overkill/gl_caller.hpp>
#include <PMS/logger.h>
#include <string>
#include <vector>

namespace overkill
{
namespace ShaderIntrospector
{

    //UNIFORM BLOCKS
    GLint  getActiveBlockCount(const GLuint program);
    const  std::vector<GLint> getUniformBlockUniformIndices(const GLuint program, const GLuint uBlockIndex);
    const  std::string getUnifromBlockName(const GLuint program, const GLint uBlockIndex);
    GLuint getUniformBlockIndex(const GLuint program, const std::string& name);

    //UNIFROMS
    GLint  getActiveUniformCount(const GLuint program);


    //ATTRIBUTES
    GLint  getActiveAttribCount(const GLuint program);
    
    GLenum printCompileStatus(const GLuint shaderid);
    GLenum printLinkStatus(const GLuint programid);
};

}
