#pragma once

#include <vector>
#include <overkill/gl_caller.hpp>

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

struct Material
{
	std::vector<UniformTexture>	maps;
	std::vector<UniformFloat>	floats;
};
