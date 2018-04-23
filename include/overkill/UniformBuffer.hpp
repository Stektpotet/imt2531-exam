#pragma once

#include <overkill/gl_caller.hpp>
#include <overkill/ShaderProgram.hpp>
#include <overkill/Config.hpp>
#include <vector>
#include <unordered_map>
#include <tuple>

namespace overkill
{

struct UniformBufferLayout
{

private:
	std::unordered_map<C::Tag, GLuint> m_uniforms;
	GLuint m_blockSize;
	GLuint m_instances;
public:
	UniformBufferLayout(GLuint instances = 1) : m_blockSize(0), m_instances(instances) {}
	
	template<GLenum type>
	void push(const C::Tag& name, GLuint componentCount = 1)
	{
		const auto size = componentCount * GLTypeSize(type);
		m_uniforms.emplace(std::make_pair(name, m_blockSize));
		m_blockSize += size;
	}
	inline GLuint blockSize()  const { return m_blockSize; }
	inline GLuint blockCount() const { return m_instances; }
	inline GLsizei size()      const { return m_blockSize * m_instances; }

	GLuint indexOfUniform(const C::Tag& name, GLuint instance = 0) const
	{
		auto search = m_uniforms.find(name); //@TODO discuss usage of at() as it works just as well here
		if (search != m_uniforms.end())
		{
			return instance * m_blockSize + search->second;
		}
		LOG_ERROR("indexOfUniform: \"%s\" cannot be found in buffer!\n has it been added in the layout?", name.c_str());
		return 0;
	}

	///TEMPLATE SPESCIALIZATIONS:
	//std140 dictates that vec3 types are placed on the GPU memory buffer as if they were vec4
	//template<>
	//void push<GL_FLOAT_VEC3>(const C::Tag& name, GLuint componentCount)
	//{
	//	const auto size = componentCount * GLTypeSize(GL_FLOAT_VEC4);
	//	m_uniforms.emplace(std::make_pair(name, UniformValue{ m_blockSize, size }));
	//	m_blockSize += size;
	//}
	//template<>
	//void push<GL_INT_VEC3>(const C::Tag& name, GLuint componentCount)
	//{
	//	const auto size = componentCount * GLTypeSize(GL_INT_VEC4);
	//	m_uniforms.emplace(std::make_pair(name, UniformValue{ m_blockSize, size }));
	//	m_blockSize += size;
	//}
	//template<>
	//void push<GL_DOUBLE_VEC3>(const C::Tag& name, GLuint componentCount)
	//{
	//	const auto size = componentCount * GLTypeSize(GL_DOUBLE_VEC4);
	//	m_uniforms.emplace(std::make_pair(name, UniformValue{ m_blockSize, size }));
	//	m_blockSize += size;
	//}
};

class UniformBuffer
{

private:
    GLuint m_id;
    C::Tag m_name;
	UniformBufferLayout m_blockLayout;
public:

	UniformBuffer(const char *name, const UniformBufferLayout& layout, const GLenum drawMode);
   
    explicit operator C::Tag() const;
    explicit operator GLuint() const;

    void clean();

    void bind() const;
    void unbind() const;

	inline GLuint blockCount()
	{ 
		return m_blockLayout.blockCount();
	}

	GLuint getUniformIndex(const C::Tag& name, const GLuint blockInstance = 0) const;
	void update(const C::ID index, GLsizeiptr size, const void *data);
};

}
