#pragma once

#include <vector>
#include <overkill/texture.hpp>
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

class Material
{
public:
	std::vector<UniformTexture>	maps;
	std::vector<UniformFloat>	floats;
    Material()=default;
    Material(const char* filepath);
};
