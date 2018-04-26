#include <overkill/ShaderProgram.hpp>
#include <overkill/ShaderIntrospector.hpp>

namespace overkill 
{

void ShaderProgram::clean()
{
    uniforms.clear();
    unbind();
    GLCall(glDeleteProgram(id));
}

ShaderProgram::operator GLuint() const
{
    return id;
}

void ShaderProgram::setMaterial(const Material& mat) const
{
 //   LOG_DEBUG("Setting material on shader: %u", id);
    bind();
    std::size_t i = 0;
    for (const auto unimap : mat.m_unimaps)
    {
        GLint location = getUniformLocation(unimap.tag);
        if (location == -1) {
            continue;
        }
        unimap.texture.bind(i);
        GLCall(glUniform1i(location, i));
        i++;
    }
    for (const auto unival : mat.m_univalues)
    {
        GLint location = getUniformLocation(unival.tag);
        if (location == -1) {
            continue;
        }
        GLCall(glUniform1f(location, unival.value));
    }
}

void ShaderProgram::bind() const
{

    GLCall(glUseProgram(id));
}

void ShaderProgram::unbind() const
{
    GLCall(glUseProgram(0));
}
GLint ShaderProgram::getUniformLocation(const std::string& name) const
{
	const auto locationIter = uniforms.find(name);
	if (locationIter == uniforms.end())
	{
		return -1;
	}
    return (*locationIter).second;
}

GLuint ShaderProgram::getUniformBlockIndex(const std::string & blockName) const
{
    GLuint index;
    GLCall(index = glGetUniformBlockIndex(id, blockName.c_str()));
    if (index == GL_INVALID_INDEX)
    {
        LOG_WARN("\nTrying to access \"%s\",\n No uniform block with that name exists!", blockName.c_str());
    }
    return index;
}

}
