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
    GLCall(glUseProgram(id));
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
    for (const auto prop : drawProperties[Enable])
    {
        GLCall(glEnable(prop));
    }
    GLCall(glBlendFunc(drawProperties[BlendFunc][0], drawProperties[BlendFunc][1]));
    GLCall(glCullFace(drawProperties[CullFace][0]));


}

void ShaderProgram::unbind() const
{
    for (const auto prop : drawProperties[Enable])
    {
        GLCall(glDisable(prop));
    }
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

}
