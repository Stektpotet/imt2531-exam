#include <overkill/ShaderProgram.hpp>
#include <overkill/ShaderIntrospector.hpp>

namespace overkill 
{

auto ShaderProgram::construct(const std::string& vert, const std::string& frag, const std::string& geom) -> GLenum
{

    const auto attachShader = [](const GLuint programId, const std::string& source, GLuint type) -> GLenum
    {   
        // Just ignore if the shader source is empty
        if (!source.empty())
        {
            GLenum err = 0;
            GLuint shaderId;
            GLCall_ReturnIfError(shaderId = glCreateShader(type));

            const char* src = source.data();
            GLCall_ReturnIfError(glShaderSource(shaderId, 1, &src, nullptr));
            GLCall_ReturnIfError(glCompileShader(shaderId));

            err = ShaderIntrospector::printCompileStatus(shaderId);
            if (err)
                return err;

            GLCall_ReturnIfError(glAttachShader(programId, shaderId));
            GLCall_ReturnIfError(glDeleteShader(shaderId)); //it may be that I'm supposed to call glDetachShader() instead.. not sure for now tho
        } 
        return 0;
    };

    GLenum err = 0;
    GLuint programId;
    GLCall_ReturnIfError( programId = glCreateProgram() );


    err = attachShader(programId, vert, GL_VERTEX_SHADER);
    if (err) 
        return err;

    err = attachShader(programId, frag, GL_FRAGMENT_SHADER);
    if (err) 
        return err;
    
    err = attachShader(programId, geom, GL_GEOMETRY_SHADER);
    if (err) 
        return err;

    GLCall_ReturnIfError(glLinkProgram(programId));
    err = ShaderIntrospector::printLinkStatus(programId);
    if (err)
        return err;

    id = programId;    // @SO FAR SO GOOD



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

        GLint location;
        GLCall(location = glGetUniformLocation(id, uniformName));
        uniforms.insert({ uniformName, location });
    }


    auto uBlockCount = ShaderIntrospector::getActiveBlockCount(id);
    uniformBlocks.reserve(uBlockCount);
    for (GLint i = 0; i < uBlockCount; i++)
    {
        auto name = ShaderIntrospector::getUnifromBlockName(id, i);
        GLuint uBlockIndex = ShaderIntrospector::getUniformBlockIndex(id, name);
        LOG_INFO("Uniform Block #%i, indexed as #%u, Name: %s", i, uBlockIndex, name.c_str());
        uniformBlocks.insert({ name, i });
        const auto& indices = ShaderIntrospector::getUniformBlockUniformIndices(id, uBlockIndex);
        for (const auto index : indices)
        {
            char* uniformName = (char*)alloca(nameMaxLength * sizeof(char));
			GLCall(glGetActiveUniform(id, index, nameMaxLength, &length, &size, &type, uniformName));
			GLsizei s = GLTypeSize(type);
			LOG_INFO("#%u has: %s, with index: %u, and of type: %u, with the size: %i\n", uBlockIndex, uniformName, index, type, s);
        }
    }
    
    GLCall(glValidateProgram(id));

    return 0;
}

ShaderProgram::ShaderProgram(const std::string& vert, const std::string& frag, const std::string& geom)
{
    construct(vert, frag, geom);
}

ShaderProgram::ShaderProgram(const std::string& filePath)
{
    auto[vert, frag, geom] = ShaderProgram::ParseProgram(filePath);
    construct(vert, frag, geom);
}

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



ShaderSource ShaderProgram::ParseProgram(const std::string& file)
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