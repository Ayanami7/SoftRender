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
		// u = u - floor(u);
		// v = v - floor(v);
		// auto u_img = u * (width - 1);
		// auto v_img = (1 - v) * (height - 1);
		// auto color = img_data.at<cv::Vec3b>(v_img, u_img);
        // return Eigen::Vector3f(color[0], color[1], color[2]);
    }
};
