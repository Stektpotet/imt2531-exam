#include <overkill/ShaderIntrospector.hpp>
namespace overkill
{

// UNIFORMS
/*
const std::vector<GLint>& ShaderIntrospector::getUniformLocations(const GLuint program)
{
    GLsizei nameMaxLength;

    GLCall(glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameMaxLength));
    LOG_INFO("Uniform name maxlength: %i", nameMaxLength);
    GLCall(glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count));
    LOG_INFO("\n\nActive uniforms: %i", count);
    for (GLuint i = 0; i < count; i++)
    {
        char* uniformName = (char*)alloca(nameMaxLength * sizeof(char));
        GLCall(glGetActiveUniform(id, i, nameMaxLength, &length, &size, &type, uniformName));
    }
    GLuint count;
    GLCall(glGetActiveUniformsiv(program, uBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &count));

    GLint indices;
    GLCall(glGetActiveUniformsiv(program, uBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &indices));
    
    std::vector<GLint> result;
    result.reserve(count);
    for (GLuint i = 0 < count; i++)
    {
        result.push_back(indices[i]);
    }
    return result;
}*/


//UNIFORM BLOCKS

GLint ShaderIntrospector::getActiveBlockCount(const GLuint program)
{
    GLint count;
    GLCall(glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &count));
    return count;
}

const std::string ShaderIntrospector::getUnifromBlockName(const GLuint program, const GLint uBlockIndex)
{
    //nameMaxLength = the longest name of a block for the given program
    GLint nameMaxLength, length;
    GLCall(glGetActiveUniformBlockiv(program, uBlockIndex, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameMaxLength));
    char* name = (char*)alloca(nameMaxLength * sizeof(char));
    GLCall(glGetActiveUniformBlockName(program, uBlockIndex, nameMaxLength, &length, name));
    return std::string(name);
}

const std::vector<GLint> ShaderIntrospector::getUniformBlockUniformIndices(const GLuint program, const GLuint uBlockIndex)
{
    GLint count;
    GLCall(glGetActiveUniformBlockiv(program, uBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &count));

    GLint* indices = (GLint*)alloca(count * sizeof(GLint));
    GLCall(glGetActiveUniformBlockiv(program, uBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices));

    std::vector<GLint> result;
    result.reserve(count);
    for (GLuint i = 0; i < count; i++)
    {
        result.push_back(indices[i]);
    }
    return result;
}

GLuint ShaderIntrospector::getUniformBlockIndex(const GLuint program, const std::string& name)
{
    GLuint index;
    GLCall(index = glGetUniformBlockIndex(program, name.c_str()));
    return index;
}

GLint ShaderIntrospector::getUniformBlockBinding(const GLuint program, GLuint uBlockIndex)
{
    GLint value;
    //GLCall(glGetActiveUniformBlockiv(program, uBlockIndex))
    return 0;
}
}