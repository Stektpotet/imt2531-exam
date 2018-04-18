#include <overkill/ShaderIntrospector.hpp>
namespace overkill
{

GLuint ShaderIntrospector::ActiveUniformBlockCount(const GLuint program)
{
    GLuint count;
    GLCall(glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &count));
    return count;
}

GLuint ShaderIntrospector::UniformBlockIndex(const GLuint program, const std::string& name)
{
    GLuint index;
    GLCall(index = glGetUniformBlockIndex(program, name.c_str()));
    return index;
}

GLint ShaderIntrospector::UniformBlockBinding(const GLuint program, GLuint uniformBlockIndex)
{
    GLint value;
    GLCall(glGetActiveUniformBlockiv(program, blockInde))
}
}