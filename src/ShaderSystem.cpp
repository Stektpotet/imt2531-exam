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

void ShaderSystem::push(const C::Tag tag, const std::string& filepath) 
{
    ShaderSystem::m_mapShaderProgramID[tag] = ShaderSystem::m_shaderPrograms.size();    
    ShaderSystem::m_shaderPrograms.emplace_back( ShaderProgram(filepath.data()) );
}

void ShaderSystem::pushUniformBuffer(const C::Tag&& tag, GLuint size)
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
GLuint ShaderSystem::getBlockUniformLocation(const C::Tag& uBlock, const C::Tag& uniform )
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



void ShaderSystem::load() 
{
    std::vector<FileEvent> fevents = Watcher::popEvents("discovered", "shaders");
    for (const auto& e : fevents) 
    {
        const auto filepath = C::ShadersFolder + ("/" + e.tag) + "." + e.extension;
        LOG_DEBUG("Shaders from file: %s", filepath.data());
        ShaderSystem::push(e.tag, filepath);
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

    lightBufferLayout.pushBlock(lightStructLayout, 8);
	//lightBufferLayout.push<GL_FLOAT_VEC4>("intensities");
	//lightBufferLayout.push<GL_FLOAT>("spread");
	//lightBufferLayout.push<GL_FLOAT>("constant");
	//lightBufferLayout.push<GL_FLOAT>("linear");
	//lightBufferLayout.push<GL_FLOAT>("quadratic");

	ShaderSystem::m_mapUniformBuffersID["OK_Lights"] = ShaderSystem::m_uniformBuffers.size(); //assign ID/index
	ShaderSystem::m_uniformBuffers.emplace_back(UniformBuffer("OK_Lights", lightBufferLayout, GL_DYNAMIC_DRAW));

	//pushUniformBuffer("OK_Lights",   sizeof(glm::vec4) * 2+sizeof(float));
	//auto buf = getUniformBufferByTag("OK_Matrices");

	GLsizei nameMaxLength;
	
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
    Watcher::discoverFiles();
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



}
