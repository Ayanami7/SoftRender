#pragma once
#include "global.h"
#include "Texture.h"
#include "Camera.h"

struct fragmentShaderPayload
{
	fragmentShaderPayload()
	{
		texture = nullptr;
	}
	fragmentShaderPayload(glm::vec3 &_color, glm::vec3 &_nor, glm::vec2 &_texc, Texture *tex)
		: color(_color), normal(_nor), tex_coords(_texc), texture(tex){}

	glm::vec3 viewPos;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 tex_coords;
	Texture *texture;
};

struct vertexShaderPayload
{
   	glm::vec3 position;
};

namespace ShaderFunc
{
	glm::vec3 vertexShader(const vertexShaderPayload &payload);

	glm::vec3 normalFragmentShader(const fragmentShaderPayload &payload, const glm::vec3& eye_pos);

	glm::vec3 textureFragmentShader(const fragmentShaderPayload &payload, const glm::vec3& eye_pos);
};
