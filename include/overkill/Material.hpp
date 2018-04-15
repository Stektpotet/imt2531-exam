#pragma once

#include <vector>
#include <overkill/texture.hpp>
#include <overkill/gl_caller.hpp>


namespace overkill 
{
    
struct UniformTexture
{
	std::string tag;
	Texture		texture;
};
struct UniformFloat
{
	std::string tag;
	float		value;
};

class Material
{
public:
	std::vector<UniformTexture>	m_unimaps;
	std::vector<UniformFloat>	m_univalues;
    Material()=default;
    Material(const std::string& tag);

};

}
