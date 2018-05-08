#pragma once

#include <vector>

#include <PMS/logger.h>
#include <overkill/Config.hpp>
#include <overkill/Texture.hpp>
#include <overkill/gl_caller.hpp>


namespace overkill 
{
    
struct UniformTexture
{
	C::Tag tag;
	Texture texture;
};

struct UniformFloat
{
	C::Tag tag;
	float  value;
};

struct UniformVec3
{
    std::string tag;
    glm::vec3 vector;
};

class Material
{
public:
    C::Tag                      m_tag;
	std::vector<UniformTexture>	m_unimaps;
	std::vector<UniformFloat>	m_univalues;
    std::vector<UniformVec3>    m_univectors;
    Material()=default;
};

}
