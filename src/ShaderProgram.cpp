#include <overkill/ShaderProgram.hpp>

namespace overkill 
{

void ShaderProgram::construct(const std::string& vert, const std::string& frag, const std::string& geom)
{
    GLCall(id = glCreateProgram());
    const auto attachShader = [this](const std::string& src, GLuint type)
    {
        if (!src.empty())
        {
            auto shader = CompileShader(type, src);
            GLCall(glAttachShader(id, shader));
            GLCall(glDeleteShader(shader)); //it may be that I'm supposed to call glDetachShader() instead.. not sure for now tho
        }
    };
    attachShader(vert, GL_VERTEX_SHADER);
    attachShader(frag, GL_FRAGMENT_SHADER);
    attachShader(geom, GL_GEOMETRY_SHADER);

    GLCall(glLinkProgram(id));
    int result;
    GLCall(glGetProgramiv(id, GL_LINK_STATUS, &result));
    if (!result)
    {
        int length;
        GLCall(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetProgramInfoLog(id, length, &length, message));
        std::cout << "\nFailed to link program: " << id << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteProgram(id));

        std::cin.get();
        return;
    }

    ///https://stackoverflow.com/questions/440144/in-opengl-is-there-a-way-to-get-a-list-of-all-uniforms-attribs-used-by-a-shade
   
    GLint count;
    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)
    GLsizei length; // name length
    
    GLsizei nameMaxLength;
    
    LOG_INFO("\n\n= = = = = =<| ATTRIBUTE INFO |>= = = = = =");

    GLCall(glGetProgramiv(id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &nameMaxLength));
    LOG_INFO("Attribute name maxlength: %i", nameMaxLength);
    GLCall(glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &count));
    LOG_INFO("Active attributes: %i", count);

    for (GLuint i = 0; i < count; i++)
    {
        char* attribName = (char*)alloca(nameMaxLength * sizeof(char));
        GLCall(glGetActiveAttrib(id, i, nameMaxLength, &length, &size, &type, attribName));
        LOG_INFO("Attribute #%u Type: %u Name: %s", i, type, attribName);
    }

    LOG_INFO("\n\n= = = = = =<| UNIFORM INFO |>= = = = = =");

    GLCall(glGetProgramiv(id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameMaxLength));
    LOG_INFO("Uniform name maxlength: %i", nameMaxLength);
    GLCall(glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count));
    LOG_INFO("\n\nActive uniforms: %i", count);

    for (GLuint i = 0; i < count; i++)
    {
        char* uniformName = (char*)alloca(nameMaxLength * sizeof(char));
        GLCall(glGetActiveUniform(id, i, nameMaxLength, &length, &size, &type, uniformName));
        LOG_INFO("Uniform #%u, Type: %u, Name: %s", i, type, uniformName);
        if (length > 15)
        {
            LOG_WARN(R"(
    Uniform #%d, %s of type %u: 
    names of uniforms should generally be shorter than 15 characters
    for optimal lookup time, if %s is not touched very often this may still be OK)",
                i, uniformName, type, uniformName
            );
        }
        GLint location;
        GLCall(location = glGetUniformLocation(id, uniformName));
        uniforms.insert({ uniformName, location });
    }

    /*GLCall(glGetProgramiv(id, GL_ACTIVE_UNIFORM_BLOCKS, &count));
    std::cout << "\nActive uniform blocks: " << count << std::endl;

    for (i = 0; i < count; i++) //GL_ACTIVE_UNIFORM_MAX_LENGTH
    {
        GLint blockCount;
        GLCall(glGetActiveUniformBlockiv(id, (GLuint)i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &blockCount));
        std::cout << "\nActive block uniforms: " << count << std::endl;

        printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
        if (length > 15)
        {
            LOG_WARN(
                R"(
        Uniform #%d, %s of type %u: 
        names of uniforms should generally be shorter than 15 characters
        for optimal lookup time, if %s is not touched very often this may still be OK
                )",
                i, name, type, name
            );
        }
        GLint location;
        GLCall(location = glGetUniformLocation(id, name));
        uniforms.insert({ name, location });
    }*/

    GLCall(glValidateProgram(id));
}

ShaderProgram::ShaderProgram(const std::string& vert, const std::string& frag, const std::string& geom)
{
    construct(vert, frag, geom);
}

ShaderProgram::ShaderProgram(const std::string& filePath)
{
    auto[vert, frag, geom] = ParseProgram(filePath);
    construct(vert, frag, geom);
}

void ShaderProgram::clean()
{
    unbind();
    GLCall(glDeleteProgram(id));
}

ShaderProgram::operator GLuint() const
{
    return id;
}

void ShaderProgram::setMaterial(const Material& mat) const
{
    LOG_DEBUG("Setting material on shader: %u", id);
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


static GLuint CompileShader(GLuint type, const std::string& source)
{
    GLuint id;
    GLCall(id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (!result)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile shader: " << ::ShaderTypeName(type) << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));

        std::cin.get();
        return 0;
    }
    return id;
}

static ShaderSource ParseProgram(const std::string& file)
{

    enum ShaderType : int { NONE = -1, VERT = 0, FRAG = 1, GEOM = 2 };

    ShaderType currentlyReading = NONE;
    std::string line;
    std::ifstream fileStream(file);
    if (!fileStream.is_open())
    {
        printf("\"%s\" could not be read...\n", file.c_str());
    }

    std::stringstream ss[3];

    for (std::size_t lineNr = 1; getline(fileStream, line); lineNr++)
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vert") != std::string::npos)
            {
                currentlyReading = VERT;
            }
            else if (line.find("frag") != std::string::npos)
            {
                currentlyReading = FRAG;
            }
            else if (line.find("geom") != std::string::npos)
            {
                currentlyReading = GEOM;
            }
            getline(fileStream, line); //instantly get #version tag
            ss[(int)currentlyReading] << line << '\n';
            ss[(int)currentlyReading] << "#line " << ++lineNr+1 << '\n'; //inject line number to get the line as is in the shader file
        }
        else
        {
            ss[(int)currentlyReading] << line << '\n';
        }
    }
    return
    ShaderSource {
        ss[(int)VERT].str(),
        ss[(int)FRAG].str(),
        ss[(int)GEOM].str()
    };
}

}