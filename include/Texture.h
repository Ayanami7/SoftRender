#pragma once
#include "global.h"
#include "stb_image.h"
#include <io.h>

class Texture 
{
private:
	//texture data
	unsigned char *image;

public:
	int width;
	int height;
	int channels;
	Texture(const std::string &path)
	{
		image = stbi_load(path.c_str(), &width, &height, &channels, 3);
		if(image == nullptr)
		{
			throw std::runtime_error("Error: load texture failed!");
			return;
		}
	}

	glm::vec3 getColor(float u, float v)
    {
		// floor the value to avoid over the range
		u = u - floor(u);
		v = v - floor(v);
		int x = static_cast<int>(u * (width - 1));
		int y = static_cast<int>((1 - v) * (height - 1));
		int index = (y * width + x) * 3;
		// calculate the x,y of 3 channels image
		float r = static_cast<float>(image[index]);
		float g = static_cast<float>(image[index + 1]);
		float b = static_cast<float>(image[index + 2]);
		return glm::vec3(r, g, b);
	}
};
