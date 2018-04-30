#pragma once

#include <overkill/gl_caller.hpp>
#include <overkill/ShaderProgram.hpp>
#include <overkill/Config.hpp>
#include <vector>
#include <unordered_map>

namespace overkill
{


struct Transform
{

	const glm::vec3 right{ 1.0f,0.0f,0.0f };
	const glm::vec3 up{ 0.0f,1.0f,0.0f };
	const glm::vec3 forward{ 0.0f,0.0f,1.0f };
	const glm::vec3 one{ 1.0f, 1.0f, 1.0f };
	const glm::vec3 zero{ 0.0f ,0.0f ,0.0f };

	glm::vec3 m_position = zero;
	glm::vec3 m_rotation = zero;
	glm::vec3 m_scale    = one;

	auto modelToWorld()->glm::mat4;
};


struct Light : public Transform
{
	glm::vec3 intensities = one; //a.k.a the color of the light
};


struct BlockLayout
{
private:
    std::unordered_map<C::Tag, GLuint>      m_vars;
    //std::unordered_map<C::Tag, BlockLayout> m_blocks;
    GLuint m_blockSize; //the combined size of this block
    C::Tag m_name;
public:
    BlockLayout(const char* name = "");
    inline GLsizei size() const { return m_blockSize; }
    explicit operator C::Tag() const;
    GLuint indexOfUniform(const C::Tag& name) const
    {
        auto search = m_vars.find(name); //@TODO discuss usage of at() as it works just as well here
        if (search != m_vars.end())
        {
            return search->second;
        }
        LOG_WARN("indexOfUniform: \"%s\" cannot be found in buffer!\n has it been added in the layout?", name.c_str());
        return 0;
        //return m_vars.at(name);
    }

    void pushBlock(const BlockLayout& block)
    {
        for (const auto& var : block.m_vars)
        {
            m_vars.emplace(std::make_pair(block.m_name + "." + var.first, m_blockSize + var.second));
        }
        m_blockSize += block.m_blockSize;
    }

    void pushBlockArray(const BlockLayout& block, GLuint count) //allows direct access to the inner-element indices
    {

        /* TESTING THE LOGIC BEHIND THIS:

            auto block0 = BlockLayout("b0")
            block0.push<16>("position");
            block0.push<16>("color");
                // block0.size = 32

            block1.push<16>("somevec");
                // block1.size = 16
            block1.pushBlock(block0);
                // somevec,     0
                // position,    16
                // color,       32
                // block1.size = 48
        */
        for (GLuint i = 0; i < count; i++)
        {
            for (const auto& var : block.m_vars)
            {
                m_vars.emplace(std::make_pair(block.m_name + '[' + std::to_string(i) + "]." + var.first, m_blockSize + var.second));
            }
            m_blockSize += block.m_blockSize;
        }
    }

    void push(const C::Tag& name, GLuint size)
    {
        m_vars.emplace(std::make_pair(name, m_blockSize));
        m_blockSize += size;
    }
};


//struct UniformBufferLayout
//{
//
//private:
//	std::unordered_map<C::Tag, GLuint> m_uniforms;
//	GLuint m_blockSize;
//    C::Tag m_name;
//public:
//	UniformBufferLayout(const char* name = "") : m_blockSize(0), m_name(name) {}
//
//	template<GLuint size>
//	void push(const C::Tag& name, GLuint count)
//	{
//	/*	m_uniforms.emplace(std::make_pair(name, m_blockSize));
//		m_blockSize += size * componentCount;*/
//        for (GLuint i = 0; i < count; i++)
//        {
//            push<size>(name + '[' + std::to_string(i) + ']');
//        }
//	}
//
//    template<GLuint size>
//    void push<size>(const C::Tag& name)
//    {
//        m_uniforms.emplace(std::make_pair(name, m_blockSize));
//        m_blockSize += size;
//    }
//
//	inline GLsizei size()      const { return m_blockSize; }
//
//	inline GLuint indexOfUniform(const C::Tag& name) const
//	{
//        return m_uniforms.at(name);
//
//		//auto search = m_uniforms.find(name); //@TODO discuss usage of at() as it works just as well here
//		//if (search != m_uniforms.end())
//		//{
//		//	return search->second;
//		//}
//		//LOG_ERROR("indexOfUniform: \"%s\" cannot be found in buffer!\n has it been added in the layout?", name.c_str());
//		//return 0;
//	}
//    
//};

class UniformBuffer
{

private:
    GLuint m_id;
    C::Tag m_name;
	BlockLayout m_blockLayout;
public:

    UniformBuffer()=default;
	UniformBuffer(const char *name, const BlockLayout& layout, const GLenum drawMode);
   
    explicit operator C::Tag() const;
    explicit operator GLuint() const;

    void clean();

    void bind() const;
    void unbind() const;

    inline GLuint blockSize()
    {
        return m_blockLayout.size();
    }

	GLuint getUniformIndex(const C::Tag& name) const;
	void update(const C::ID index, GLsizeiptr size, const void *data);
};

}
