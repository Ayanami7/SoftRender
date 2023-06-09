#pragma once
#include "global.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include <map>
#include <cmath>

struct fragmentShaderPayload
{
	fragmentShaderPayload()
	{
		texture = nullptr;
	}
	fragmentShaderPayload(glm::vec3 &_color, glm::vec3 &_nor, glm::vec2 &_texc, Texture *tex)
		: color(_color), normal(_nor), tex_coords(_texc), texture(tex){}

	glm::vec3 viewPos;			// view position
	glm::vec3 color;			// color
	glm::vec3 normal;			// normal vector
	glm::vec2 tex_coords;		// u,v position of texture
	glm::vec3 cameraPos;		// eye position
	glm::vec3 ambLight;			// Ambient light
	Texture *texture;			// texture pointer
	std::map<std::string, Light *> lights;	// Point Light set
};

struct vertexShaderPayload
{
   	glm::vec3 position;
};

namespace ShaderFunc
{
	glm::vec3 vertexShader(const vertexShaderPayload &payload);

	glm::vec3 normalFragmentShader(const fragmentShaderPayload &payload);

	glm::vec3 textureFragmentShader(const fragmentShaderPayload &payload);

	glm::vec3 phongFragmentShader(const fragmentShaderPayload &payload);

	glm::vec3 bumpFragmentShader(const fragmentShaderPayload &payload);

	glm::vec3 displacementFragmentShader(const fragmentShaderPayload &payload);
};
