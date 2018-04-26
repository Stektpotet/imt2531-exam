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
            GLuint uBlockIndex = ShaderIntrospector::getUniformBlockIndex(id, name);
            LOG_INFO("Uniform Block #%i, indexed as #%u, Name: %s", i, uBlockIndex, name.c_str());
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
    err = ShaderSystem::parseProgram(shaderString, &vert, &frag, &geom);
    if (err){
        LOG_WARN("ShaderSystem::parseProgram failed, using default shaderprogram..");
        ShaderSystem::m_mapShaderProgramID[tag] = 0;
        return;
    }

    ShaderProgram program;
    err = ShaderSystem::makeProgram(vert, frag, geom, &program);
    if (err) {
        LOG_WARN("ShaderSystem::makeProgram failed, using default shaderprogram..");
        ShaderSystem::m_mapShaderProgramID[tag] = 0;   
        return;
    }

    ShaderSystem::m_mapShaderProgramID[tag] = ShaderSystem::m_shaderPrograms.size();    
    ShaderSystem::m_shaderPrograms.emplace_back( program );

    return;
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

    //pushUniformBuffer("OK_Lights", sizeof(LightData) * 8);
	auto matBufferLayout = BlockLayout();
	matBufferLayout.push("projection", 64);
	matBufferLayout.push("view", 64);
	matBufferLayout.push("view_position", 16);	
	
	//pushUniformBuffer("OK_Matrices",    sizeof(glm::mat4) * 2);
	ShaderSystem::m_mapUniformBuffersID["OK_Matrices"] = ShaderSystem::m_uniformBuffers.size(); //assign ID/index
	auto buf = ShaderSystem::m_uniformBuffers.emplace_back(UniformBuffer("OK_Matrices", matBufferLayout, GL_DYNAMIC_DRAW));

	auto lightBufferLayout = BlockLayout();
    //lightBufferLayout.push<16>("light.position")
    auto lightStructLayout = BlockLayout("light");
    lightStructLayout.push("position", 16);
    lightStructLayout.push("intensities", 16);
    lightStructLayout.push("constant", 4);
    lightStructLayout.push("linear", 4);
    lightStructLayout.push("quadratic", 4);
    lightStructLayout.push("alignment", 4);

    //@REF: http://sunandblackcat.com/tipFullView.php?l=eng&topicid=21&topic=OpenGL-Uniform-Buffer-Objects
    /*@NOTE:
    If you are using std140 format, then you can manually calculate offset for each uniform variable. 
    Locations of uniform variables must be aligned in memory relative to the start of the buffer. 
    If alignment for type of uniform variable is 16 bytes, 
    then uniform variable can be placed only on bytes equal to multiple of 16 - 0, 16, 32, 48, etc. 
    std140 layout rules for different types of data in GLSL:
    */


    lightBufferLayout.pushBlock(lightStructLayout, 8);

	ShaderSystem::m_mapUniformBuffersID["OK_Lights"] = ShaderSystem::m_uniformBuffers.size(); //assign ID/index
	ShaderSystem::m_uniformBuffers.emplace_back(UniformBuffer("OK_Lights", lightBufferLayout, GL_DYNAMIC_DRAW));

	//pushUniformBuffer("OK_Lights",   sizeof(glm::vec4) * 2+sizeof(float));
	//auto buf = getUniformBufferByTag("OK_Matrices");

	
	linkUniformBlocksForAll();
	GLuint bindPoint = 0;
	for (auto& buffer : m_uniformBuffers)
	{
        buffer.bind();
		for (const auto& shader : m_shaderPrograms)
		{
			/*if (buffer.blockCount() == 1)
			{*/
				auto uBlockIndex = ShaderIntrospector::getUniformBlockIndex(GLuint(shader), C::Tag(buffer));
				if (uBlockIndex != GL_INVALID_INDEX) //this block did exist in the shader
				{
					GLCall(glUniformBlockBinding(GLuint(shader), uBlockIndex, bindPoint));
					GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, GLuint(buffer)));
				}
			//}
			//else
			//{
			//	//for (int blockInstance = 0; blockInstance < buffer.blockCount(); blockInstance++)
			//	//{
			//	//	const auto& blockName = C::Tag(buffer) + '[' + std::to_string(blockInstance) + ']';
			//	//	auto uBlockIndex = ShaderIntrospector::getUniformBlockIndex(GLuint(shader), blockName);
			//	//	if (uBlockIndex != GL_INVALID_INDEX) //this block did exist in the shader
			//	//	{
			//	//		GLCall(glUniformBlockBinding(GLuint(shader), uBlockIndex, bindPoint));


   // //                    GLint offsetAlignment;
   // //                    GLCall(glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &offsetAlignment));
   // //                    GLint bufferBindings;
   // //                    GLCall(glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &bufferBindings));
   // //                    LOG_INFO("bindingMAX: %i", bufferBindings);
   // //                    

   // //                    GLint bufSize;
   // //                    glGetBufferParameteriv(GL_UNIFORM_BUFFER, GL_BUFFER_SIZE, &bufSize);
   // //                    LOG_INFO("Block: #%i,\toffset: %u,\tsize: %u,\tBufferSize: %i,\toffset alignment: %i", blockInstance, a, buffer.blockSize(), bufSize, offsetAlignment);
   // //                    //GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, bindPoint, GLuint(buffer), a, buffer.blockSize()));
			//	//		//GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, GLuint(buffer)));
   // //                    
			//	//	}
			//	//}
   //             GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, GLuint(buffer)));
			//}
		}
        buffer.unbind();
		bindPoint++;
	}
}

void ShaderSystem::reload() 
{

    // Delete from GPU
    for (auto shaderprog : ShaderSystem::m_shaderPrograms) 
    {
        shaderprog.clean();         
    }
	// Remove all shaders
	ShaderSystem::m_shaderPrograms.clear();

	// Delete from GPU
	for (auto uBuffer : ShaderSystem::m_uniformBuffers)
	{
		uBuffer.clean();
	}
	// Remove all uniformBuffers
	ShaderSystem::m_uniformBuffers.clear();


    // Load from file again
    Watcher::pollEvents();
    ShaderSystem::load();
    for(auto onUpdate : ShaderSystem::m_updateCallbacks)
    {
        auto shaderID =  ShaderSystem::getIdByTag(onUpdate.tag);
        onUpdate.callback(shaderID, onUpdate.modelID, onUpdate.meshID);
    }
}

void ShaderSystem::linkUniformBlocksForAll()
{   
    GLuint bindPoint = 0;
    for (const auto& shader : m_shaderPrograms)
    {
        auto uBlockCount = ShaderIntrospector::getActiveBlockCount(GLuint(shader));
        for (GLint i = 0; i < uBlockCount; i++)
        {
            auto name = ShaderIntrospector::getUnifromBlockName(GLuint(shader), i);
            LOG_INFO("Uniform Block #%i, Name: %s", i, name.c_str());
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


        //GLCall(glUniformBlockBinding(GLuint(shader), shader.getUniformBlockIndex(C::Tag(uBuffer)), bindPoint));
        //GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, GLuint(uBuffer)));
    }
    bindPoint++;
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
                                std::string* outGeom) -> C::Err 
{

    enum ShaderType : int { NONE = -1, VERT = 0, FRAG = 1, GEOM = 2 };

    ShaderType currentlyReading = NONE;

    std::string line;
    std::stringstream fileStream(fileString);
    
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
    
    *outVert = ss[(int)VERT].str();
    *outFrag = ss[(int)FRAG].str();
    *outGeom = ss[(int)GEOM].str();
    return 0;
}

auto ShaderSystem::makeProgram(const std::string& vert, 
                               const std::string& frag, 
                               const std::string& geom, 
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

    *outProgram = program;
    return 0;
}

}
