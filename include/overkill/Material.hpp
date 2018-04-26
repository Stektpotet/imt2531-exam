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

class Material
{
public:
	std::vector<UniformTexture>	m_unimaps;
	std::vector<UniformFloat>	m_univalues;
    Material()=default;
};

}
