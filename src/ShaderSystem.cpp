#include <overkill/ShaderSystem.hpp>

namespace overkill 
{
std::unordered_map<C::Tag, std::set<GLuint>> ShaderSystem::m_mapUniformBufferTargets;
std::vector<UniformBuffer>                   ShaderSystem::m_uniformBuffers;
std::unordered_map<C::Tag, C::ID>            ShaderSystem::m_mapUniformBuffersID;
std::vector<ShaderProgram>                   ShaderSystem::m_shaderPrograms;
std::unordered_map<C::Tag, C::ID>            ShaderSystem::m_mapShaderProgramID;
std::vector<ShaderSystem::UpdateCallback>    ShaderSystem::m_updateCallbacks;

auto ShaderSystem::getIdByTag(const C::Tag& tag) -> C::ID
{
    return m_mapShaderProgramID[tag];
}

auto ShaderSystem::getByTag(const C::Tag& tag) -> const ShaderProgram&
{
    return ShaderSystem::m_shaderPrograms[m_mapShaderProgramID[tag]];
}

auto ShaderSystem::getById(C::ID shaderProgramID) -> const ShaderProgram&
{
    return ShaderSystem::m_shaderPrograms[shaderProgramID];
}

auto ShaderSystem::copyByTag(const C::Tag& tag) -> ShaderProgram 
{
    return ShaderSystem::m_shaderPrograms[m_mapShaderProgramID[tag]];
}

auto ShaderSystem::copyById(C::ID shaderProgramID) -> ShaderProgram
{
    return ShaderSystem::m_shaderPrograms[shaderProgramID];
}


void ShaderSystem::pushUniformBuffer(const C::Tag&& /*tag*/, GLuint /*size*/)
{
    //ShaderSystem::m_mapUniformBufferDynamic[tag] = ShaderSystem::m_uniformBuffers.size();
    //ShaderSystem::m_uniformBuffers.emplace_back( UniformBuffer(tag.c_str(), nullptr, size, GL_DYNAMIC_DRAW) );
}

void ShaderSystem::linkUniformBlocks()
{

    std::unordered_map<C::Tag, GLint> uniformBlocks;

    for (auto& shader : m_shaderPrograms)
    {
        auto id = GLuint(shader);
        auto uBlockCount = ShaderIntrospector::getActiveBlockCount(id);
        uniformBlocks.reserve(uBlockCount);
        for (GLint i = 0; i < uBlockCount; i++)
        {
            auto name = ShaderIntrospector::getUnifromBlockName(id, i);
         //   LOG_INFO("Uniform Block #%i, indexed as #%u, Name: %s", i, uBlockIndex, name.c_str());
			auto search = m_mapUniformBufferTargets.find(name);
			if (search != m_mapUniformBufferTargets.end())
			{
				search->second.insert(GLuint(shader));
			}
			else
			{
				m_mapUniformBufferTargets.insert({ name,{ GLuint(shader) } });
			}
            uniformBlocks.insert({ name, i });
            /*ShaderSystem::m_mapUniformBufferTargets.
            const auto& indices = ShaderIntrospector::getUniformBlockUniformIndices(id, uBlockIndex);
            for (const auto index : indices)
            {
                char* uniformName = (char*)alloca(nameMaxLength * sizeof(char));
                GLsizei length;
                GLCall(glGetActiveUniformName(id, index, nameMaxLength, &length, uniformName));
                LOG_INFO("#%u has: %s", uBlockIndex, uniformName);
            }*/
        }
    }
}

/// <summary>
///
///
///
///
GLuint ShaderSystem::getBlockUniformLocation(const C::Tag& /*uBlock*/, const C::Tag& /*uniform*/ )
{
	/*for (auto& uBuffer : m_uniformBuffers)
	{
		uBuffer.holdsBlock(uBlock)
	}*/
	return 0;
}

auto ShaderSystem::getUniformBufferIdByTag(const C::Tag& tag) -> C::ID
{
    return ShaderSystem::m_mapUniformBuffersID[tag];
}

auto ShaderSystem::getUniformBufferByTag(const C::Tag& tag) -> const UniformBuffer&
{
    return ShaderSystem::m_uniformBuffers[m_mapUniformBuffersID[tag]];
}

auto ShaderSystem::getUniformBufferById(C::ID uBufferID) -> const UniformBuffer&
{
    return ShaderSystem::m_uniformBuffers[uBufferID];
}

auto ShaderSystem::getUniformBuffer(const C::Tag& tag) -> UniformBuffer 
{
    return ShaderSystem::m_uniformBuffers[m_mapUniformBuffersID[tag]];
}

void ShaderSystem::push(const C::Tag tag, const std::string& filepath)
{
    C::Err err;

    std::string shaderString;
    err = Util::fileToString(filepath, &shaderString);
    if (err) {
        LOG_WARN("Util::fileToString(%s) failed, using default shaderprogram..", filepath.data());
        ShaderSystem::m_mapShaderProgramID[tag] = 0;
        return;
    }

    std::string vert, frag, geom;
    std::array<std::vector<GLenum>, 3> drawProperties;
    err = ShaderSystem::parseProgram(shaderString, &vert, &frag, &geom, &drawProperties);
    if (err){
        LOG_WARN("ShaderSystem::parseProgram failed, using default shaderprogram..");
        ShaderSystem::m_mapShaderProgramID[tag] = 0;
        return;
    }

    ShaderProgram program;
    err = ShaderSystem::makeProgram(vert, frag, geom, drawProperties, &program);
    if (err) {
        LOG_WARN("ShaderSystem::makeProgram failed, using default shaderprogram..");
        ShaderSystem::m_mapShaderProgramID[tag] = 0;   
        return;
    }

    ShaderSystem::m_mapShaderProgramID[tag] = ShaderSystem::m_shaderPrograms.size();    
    program.m_tag = tag;
    ShaderSystem::m_shaderPrograms.emplace_back( program );


    return;
}

void ShaderSystem::createUniformBuffers()
{

    auto matBufferLayout = BlockLayout();
    matBufferLayout.push("projection", 64);
    matBufferLayout.push("view", 64);
    matBufferLayout.push("view_position", 16);

    ShaderSystem::m_mapUniformBuffersID["OK_Matrices"] = ShaderSystem::m_uniformBuffers.size(); //assign ID/index
    ShaderSystem::m_uniformBuffers.emplace_back(UniformBuffer("OK_Matrices", matBufferLayout, GL_DYNAMIC_DRAW));

    auto pointLightStructLayout = BlockLayout("light");
    pointLightStructLayout.push("position", 16);
    pointLightStructLayout.push("intensities", 16);
    pointLightStructLayout.push("constant", 4);
    pointLightStructLayout.push("linear", 4);
    pointLightStructLayout.push("quadratic", 4);
    pointLightStructLayout.push("alignment", 4);

    auto sunLightStructLayout = BlockLayout("sun");
    sunLightStructLayout.push("direction", 16);
    sunLightStructLayout.push("intensities", 16);

    //@REF: http://sunandblackcat.com/tipFullView.php?l=eng&topicid=21&topic=OpenGL-Uniform-Buffer-Objects
    /*@NOTE:
    If you are using std140 format, then you can manually calculate offset for each uniform variable.
    Locations of uniform variables must be aligned in memory relative to the start of the buffer.
    If alignment for type of uniform variable is 16 bytes,
    then uniform variable can be placed only on bytes equal to multiple of 16 - 0, 16, 32, 48, etc.
    std140 layout rules for different types of data in GLSL:
    */

    auto lightBufferLayout = BlockLayout();
    lightBufferLayout.pushBlockArray(pointLightStructLayout, C::MAX_LIGHTS); //order of pushing these blocks matter!!!!!
    lightBufferLayout.pushBlock(sunLightStructLayout);

    ShaderSystem::m_mapUniformBuffersID["OK_Lights"] = ShaderSystem::m_uniformBuffers.size(); //assign ID/index
    ShaderSystem::m_uniformBuffers.emplace_back(UniformBuffer("OK_Lights", lightBufferLayout, GL_DYNAMIC_DRAW));
}

void ShaderSystem::load() 
{
    std::vector<FileEvent> fevents = Watcher::popEvents("discovered", "shaders");
    
    LOG_DEBUG("Shaders from file: assets/shaders/_default.glsl");    
    ShaderSystem::push("_default", "assets/shaders/_default.glsl");

    for (const auto& e : fevents) 
    {
        // skip _default shader as it has been loaded already
        if (e.tag == "_default") continue; 

        const auto filepath = C::ShadersFolder + ("/" + e.tag) + "." + e.extension;
        LOG_DEBUG("Shaders from file: %s", filepath.data());
        ShaderSystem::push(e.tag, filepath);
    }
    
    if (ShaderSystem::m_shaderPrograms.size() == 0) {
        LOG_ERROR("ShaderSystem could not load any shader programs");
    }

	enlistUniformBlockTargets();

    //
    // Link all shaders with their needed uniform buffers i.e. set up binding points and data range
    //
    {
	    GLuint bindPoint = 0;
	    for (auto& buffer : m_uniformBuffers)
	    {
            buffer.bind();
	    	for (const auto& shader : m_shaderPrograms)
	    	{
	    		auto uBlockIndex = ShaderIntrospector::getUniformBlockIndex(GLuint(shader), C::Tag(buffer));
	    		if (uBlockIndex != GL_INVALID_INDEX) //this block did exist in the shader
	    		{
	    			GLCall(glUniformBlockBinding(GLuint(shader), uBlockIndex, bindPoint));  //link blocks of the same type to the same binding point
	    			GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, GLuint(buffer))); //reserve data range on the uniform buffer
	    		}
	    	}
            buffer.unbind();
	    	bindPoint++;
	    }
    }
}


void ShaderSystem::clean() 
{
    // Delete from GPU
    for (auto shaderprog : ShaderSystem::m_shaderPrograms)
    {
        shaderprog.clean();
    }
    // Remove all shaders
    ShaderSystem::m_shaderPrograms.clear();

}

void ShaderSystem::reload() 
{
    ShaderSystem::clean();

	//// Delete from GPU
	//for (auto uBuffer : ShaderSystem::m_uniformBuffers)
	//{
	//	uBuffer.clean();
	//}
	//// Remove all uniformBuffers
	//ShaderSystem::m_uniformBuffers.clear();


    // Load from file again
    Watcher::pollEvents();
    ShaderSystem::load();
    for(auto onUpdate : ShaderSystem::m_updateCallbacks)
    {
        auto shaderID =  ShaderSystem::getIdByTag(onUpdate.tag);
        onUpdate.callback(shaderID, onUpdate.modelID, onUpdate.meshID);
    }
}

void ShaderSystem::enlistUniformBlockTargets()
{   
    for (const auto& shader : m_shaderPrograms)
    {
        auto uBlockCount = ShaderIntrospector::getActiveBlockCount(GLuint(shader));
        for (GLint i = 0; i < uBlockCount; i++)
        {
            auto name = ShaderIntrospector::getUnifromBlockName(GLuint(shader), i);
           // LOG_INFO("Uniform Block #%i, Name: %s", i, name.c_str());
            auto search = m_mapUniformBufferTargets.find(name);
            if (search != m_mapUniformBufferTargets.end())
            {
                search->second.insert(GLuint(shader));
            }
            else
            {
                m_mapUniformBufferTargets.insert({ name, {GLuint(shader)} });
            }
        }
    }
}

void ShaderSystem::bindOnUpdate(const C::Tag& shaderTag, C::ID modelID, C::ID meshID, OnUpdate onUpdate)
{
    ShaderSystem::m_updateCallbacks.emplace_back(
        UpdateCallback{ 
            shaderTag,
            modelID,
            meshID, 
            onUpdate 
        }
    );
}

void ShaderSystem::unbindAll()
{
    ShaderSystem::m_updateCallbacks.clear();
}




auto ShaderSystem::parseProgram(const std::string& fileString, 
                                std::string* outVert, 
                                std::string* outFrag, 
                                std::string* outGeom,
                                std::array<std::vector<GLenum>, 3>* drawProperties
                                ) -> C::Err
{

    enum ShaderType : int { NONE = -1, VERT = 0, FRAG = 1, GEOM = 2 };

    ShaderType currentlyReading = NONE;

    std::string line;
    std::stringstream fileStream(fileString);
    //defualt properties
    std::array<std::vector<GLenum>, 3> drawProps;

    drawProps[ShaderProgram::Enable] =    { GL_BLEND, GL_DEPTH_TEST, GL_CULL_FACE };
    drawProps[ShaderProgram::BlendFunc] = { GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA };
    drawProps[ShaderProgram::CullFace] =  { GL_BACK };





    std::stringstream ss[3];

    for (std::size_t lineNr = 1; getline(fileStream, line); lineNr++)
    {
        if (line.find("#prop ") != std::string::npos)
        {
            if (line.find("ZTest ") != std::string::npos)
            {
                auto offCases = std::array<std::string, 4> {{ "OFF", "FALSE", "0", "DISABLED" }};
                for (const auto& str : offCases)
                {
                    const auto& it = std::search(line.begin() + 12, line.end(),
                        str.begin(), str.end(),
                        [](char ch1, char ch2) { return ::toupper(ch1) == ::toupper(ch2); }
                    );
                    if (it != line.end())
                    {
                        LOG_INFO("ShaderProgram:  Will not use Depth Test, to turn it on, add \"#prop ZTest On | true | 1 | Enabled\" to your program");
                        drawProps[ShaderProgram::Enable].erase(
                            std::remove(
                                drawProps[ShaderProgram::Enable].begin(),
                                drawProps[ShaderProgram::Enable].end(),
                                GL_DEPTH_TEST
                            ),
                            drawProps[ShaderProgram::Enable].end()
                        );
                        break;
                    }
                }
            }
            else if (line.find("Blend ") != std::string::npos)
            {
                std::array<std::string, 4> offCases = { {"OFF", "FALSE", "0", "DISABLED"} };
                for (const auto& str : offCases)
                {
                    const auto& it = std::search(line.begin() + 12, line.end(),
                        str.begin(), str.end(),
                        [](char ch1, char ch2) { return ::toupper(ch1) == ::toupper(ch2); }
                    );
                    if (it != line.end())
                    {
                        LOG_INFO("\nShaderProgram: Will not use Blending, to turn it on, add \"#prop Blend On | true | 1 | Enabled\" to your program");
                        drawProps[ShaderProgram::Enable].erase(
                            std::remove(
                                drawProps[ShaderProgram::Enable].begin(),
                                drawProps[ShaderProgram::Enable].end(),
                                GL_BLEND
                            ),
                            drawProps[ShaderProgram::Enable].end()
                        );
                    }
                }
                std::array<std::string, 15> blendParam = {{
                    "ONEMINUSCONSTANTALPHA", 
                    "ONEMINUSCONSTANTCOLOR", 
                    "ONEMINUSDSTALPHA", 
                    "ONEMINUSSRCCOLOR", 
                    "ONEMINUSDSTCOLOR", 
                    "ONEMINUSSRCALPHA", 
                    "SRCALPHASATURATE" 
                    "CONSTANTCOLOR", 
                    "CONSTANTALPHA",
                    "ZERO", 
                    "ONE", 
                    "SRCCOLOR", 
                    "DSTCOLOR", 
                    "SRCALPHA", 
                    "DSTALPHA", 
                }};
                int i = 0;
                int start = 11;
                for (const auto& str : blendParam)
                {
                    const auto& it = std::search(line.begin() + start, line.end(),
                        str.begin(), str.end(),
                        [](char ch1, char ch2) { return ::toupper(ch1) == ::toupper(ch2); }
                    );
                    if (it != line.end())
                    {
                        start = it - line.begin();
                        switch (i)
                        {
                        case 0:
                            drawProps[ShaderProgram::BlendFunc][0] = GL_ZERO; break;
                        case 1:
                            drawProps[ShaderProgram::BlendFunc][0] = GL_ONE; break;
                        case 2:
                            drawProps[ShaderProgram::BlendFunc][0] = GL_SRC_COLOR; break;
                        case 3:
                            drawProps[ShaderProgram::BlendFunc][0] = GL_ONE_MINUS_SRC_COLOR; break;
                        case 4:
                            drawProps[ShaderProgram::BlendFunc][0] = GL_DST_COLOR ; break;
                        case 5:
                            drawProps[ShaderProgram::BlendFunc][0] = GL_ONE_MINUS_DST_COLOR; break;
                        case 6:
                            drawProps[ShaderProgram::BlendFunc][0] = GL_SRC_ALPHA; break;
                        case 7:
                            drawProps[ShaderProgram::BlendFunc][0] = GL_ONE_MINUS_SRC_ALPHA; break;
                        case 8:
                            drawProps[ShaderProgram::BlendFunc][0] = GL_DST_ALPHA; break;
                        case 9:
                            drawProps[ShaderProgram::BlendFunc][0] = GL_ONE_MINUS_DST_ALPHA ; break;
                        case 10:
                            drawProps[ShaderProgram::BlendFunc][0] = GL_CONSTANT_COLOR; break;
                        case 11:
                            drawProps[ShaderProgram::BlendFunc][0] = GL_ONE_MINUS_CONSTANT_COLOR; break;
                        case 12:
                            drawProps[ShaderProgram::BlendFunc][0] = GL_CONSTANT_ALPHA; break;
                        case 13:
                            drawProps[ShaderProgram::BlendFunc][0] = GL_ONE_MINUS_CONSTANT_ALPHA ; break;
                        case 14:
                            drawProps[ShaderProgram::BlendFunc][0] = GL_SRC_ALPHA_SATURATE; break;
                        }
                        break;
                    }
                    i++;
                }
                i = 0;
                for (const auto& str : blendParam)
                {
                    const auto& it = std::search(line.begin() + start, line.end(),
                        str.begin(), str.end(),
                        [](char ch1, char ch2) { return ::toupper(ch1) == ::toupper(ch2); }
                    );
                    if (it != line.end())
                    {
                        switch (i)
                        {
                        case 0:
                            drawProps[ShaderProgram::BlendFunc][1] = GL_ZERO; break;
                        case 1:
                            drawProps[ShaderProgram::BlendFunc][1] = GL_ONE; break;
                        case 2:
                            drawProps[ShaderProgram::BlendFunc][1] = GL_SRC_COLOR; break;
                        case 3:
                            drawProps[ShaderProgram::BlendFunc][1] = GL_ONE_MINUS_SRC_COLOR; break;
                        case 4:
                            drawProps[ShaderProgram::BlendFunc][1] = GL_DST_COLOR; break;
                        case 5:
                            drawProps[ShaderProgram::BlendFunc][1] = GL_ONE_MINUS_DST_COLOR; break;
                        case 6:
                            drawProps[ShaderProgram::BlendFunc][1] = GL_SRC_ALPHA; break;
                        case 7:
                            drawProps[ShaderProgram::BlendFunc][1] = GL_ONE_MINUS_SRC_ALPHA; break;
                        case 8:
                            drawProps[ShaderProgram::BlendFunc][1] = GL_DST_ALPHA; break;
                        case 9:
                            drawProps[ShaderProgram::BlendFunc][1] = GL_ONE_MINUS_DST_ALPHA; break;
                        case 10:
                            drawProps[ShaderProgram::BlendFunc][1] = GL_CONSTANT_COLOR; break;
                        case 11:
                            drawProps[ShaderProgram::BlendFunc][1] = GL_ONE_MINUS_CONSTANT_COLOR; break;
                        case 12:
                            drawProps[ShaderProgram::BlendFunc][1] = GL_CONSTANT_ALPHA; break;
                        case 13:
                            drawProps[ShaderProgram::BlendFunc][1] = GL_ONE_MINUS_CONSTANT_ALPHA; break;
                        case 14:
                            drawProps[ShaderProgram::BlendFunc][1] = GL_SRC_ALPHA_SATURATE; break;
                        }
                        break;
                    }
                    i++;
                }
            }
            if (line.find("Cull ") != std::string::npos)
            {
                std::array<std::string, 4> offCases = { {"OFF", "FALSE", "0", "DISABLED"} };
                for (const auto& str : offCases)
                {
                    const auto& it = std::search(line.begin() + 12, line.end(),
                        str.begin(), str.end(),
                        [](char ch1, char ch2) { return ::toupper(ch1) == ::toupper(ch2); }
                    );
                    if (it != line.end())
                    {
                        LOG_INFO("\nShaderProgram: Will not use Blending, to turn it on, add \"#prop Blend On | true | 1 | Enabled\" to your program");
                        drawProps[ShaderProgram::Enable].erase(
                            std::remove(
                                drawProps[ShaderProgram::Enable].begin(),
                                drawProps[ShaderProgram::Enable].end(),
                                GL_CULL_FACE
                            ),
                            drawProps[ShaderProgram::Enable].end()
                        );
                        break;
                    }
                    std::array<std::string, 3> cullFaces = { {"FRONT", "BACK", "BOTH"} };
                int i = 0;
                for (const auto& str : cullFaces)
                {
                    const auto& it = std::search(line.begin() + 11, line.end(),
                        str.begin(), str.end(),
                        [](char ch1, char ch2) { return ::toupper(ch1) == ::toupper(ch2); }
                    );
                    if (it != line.end())
                    {
                        switch (i)
                        {
                        case 0:
                            drawProps[ShaderProgram::CullFace] = { GL_FRONT }; break;
                        case 1:
                            drawProps[ShaderProgram::CullFace] = { GL_BACK }; break;
                        case 2:
                            drawProps[ShaderProgram::CullFace] = { GL_FRONT_AND_BACK }; break;
                        }
                        break;
                    }
                    i++;
                }
                }
                std::array<std::string, 3> cullFaces = { {"FRONT", "BACK", "BOTH" }};
                int i = 0;
                for (const auto& str : cullFaces)
                {
                    const auto& it = std::search(line.begin() + 11, line.end(),
                        str.begin(), str.end(),
                        [](char ch1, char ch2) { return ::toupper(ch1) == ::toupper(ch2); }
                    );
                    if (it != line.end())
                    {
                        switch (i)
                        {
                        case 0:
                            drawProps[ShaderProgram::CullFace] = { GL_FRONT }; break;
                        case 1:
                            drawProps[ShaderProgram::CullFace] = { GL_BACK }; break;
                        case 2:
                            drawProps[ShaderProgram::CullFace] = { GL_FRONT_AND_BACK }; break;
                        }
                        break;
                    }
                    i++;
                }
            }
        }
        else if (line.find("#shader ") != std::string::npos)
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
            if (currentlyReading == NONE)
            {
                LOG_ERROR("Trying to read %s into NONE. Has the shader been set with the \"#shader\" directive?", fileString.c_str());
            }
            ss[(int)currentlyReading] << line << '\n';
        }
    }
    
    *outVert = ss[(int)VERT].str();
    *outFrag = ss[(int)FRAG].str();
    *outGeom = ss[(int)GEOM].str();
    *drawProperties = drawProps;
    return 0;
}

auto ShaderSystem::makeProgram(const std::string& vert, 
                               const std::string& frag, 
                               const std::string& geom,
                               const std::array<std::vector<GLenum>, 3>& drawProperties,
                               ShaderProgram* outProgram) -> C::Err
{
    const auto attachShader = [](const GLuint programId, const std::string& source, GLuint type) -> C::Err
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

            err = ShaderIntrospector::checkCompileStatus(shaderId);
            if (err)
                return err;

            GLCall_ReturnIfError(glAttachShader(programId, shaderId));
            GLCall_ReturnIfError(glDeleteShader(shaderId)); //it may be that I'm supposed to call glDetachShader() instead.. not sure for now tho
        } 
        return 0;
    };

    GLenum err = 0;
    ShaderProgram program;

    GLCall( program.id = glCreateProgram() );


    err = attachShader(program.id, vert, GL_VERTEX_SHADER);
    if (err) 
        return err;

    err = attachShader(program.id, frag, GL_FRAGMENT_SHADER);
    if (err) 
        return err;
    
    err = attachShader(program.id, geom, GL_GEOMETRY_SHADER);
    if (err) 
        return err;

    GLCall_ReturnIfError(glLinkProgram(program.id));
    err = ShaderIntrospector::checkLinkStatus(program.id);
    if (err)
        return err;

 

    ///https://stackoverflow.com/questions/440144/in-opengl-is-there-a-way-to-get-a-list-of-all-uniforms-attribs-used-by-a-shade
    //
    // LOG active shader attributes
    //   
    {
        GLsizei nameMaxLength = ShaderIntrospector::getAttributeMaxNameLength(program.id);
        GLint count = ShaderIntrospector::getActiveAttribCount(program.id);

        LOG_INFO("Active attributes: %i", count);

        for (GLint i = 0; i < count; i++)
        {
            std::string name = ShaderIntrospector::getAttribName(program.id, i, nameMaxLength);
        }        
    }


    //
    // Cache single uniforms
    //
    {
        GLsizei nameMaxLength = ShaderIntrospector::getUniformMaxNameLength(program.id);
        GLint count = ShaderIntrospector::getActiveUniformCount(program.id);
        
        LOG_INFO("Active uniforms: %i", count);

        for (GLint i = 0; i < count; i++)
        {
            std::string uniformName = ShaderIntrospector::getUniformName(program.id, i, nameMaxLength);
         
            GLint location;
            GLCall(location = glGetUniformLocation(program.id, uniformName.data()));
            
            program.uniforms.insert({ uniformName, location });
        }
    }

    //
    // Cache uniforms for each uniform block
    //
    {
        GLsizei nameMaxLength = ShaderIntrospector::getUniformMaxNameLength(program.id);
        auto uBlockCount = ShaderIntrospector::getActiveBlockCount(program.id);
    
        program.uniformBlocks.reserve(uBlockCount);
    
        for (GLint i = 0; i < uBlockCount; i++)
        {
            auto name = ShaderIntrospector::getUnifromBlockName(program.id, i);
            GLuint uBlockIndex = ShaderIntrospector::getUniformBlockIndex(program.id, name);
       //     LOG_INFO("Uniform Block #%i, indexed as #%u, Name: %s", i, uBlockIndex, name.c_str());
            
            program.uniformBlocks.insert({ name, i });
            
            const auto& indices = ShaderIntrospector::getUniformBlockUniformIndices(program.id, uBlockIndex);
            for (const auto index : indices)
            {
                std::string uniformName = ShaderIntrospector::getUniformName(program.id, index, nameMaxLength);
            //    LOG_DEBUG("#%u has: %s, with index: %u\n", uBlockIndex, uniformName.data(), index);
            }
        }    
    }

    GLCall_ReturnIfError(glValidateProgram(program.id));

    program.drawProperties = drawProperties;

    *outProgram = program;
    return 0;
}

}
