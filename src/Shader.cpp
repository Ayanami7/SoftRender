#include "Shader.h"

glm::vec3 ShaderFunc::vertexShader(const vertexShaderPayload &payload)
{
	return payload.position;
}


glm::vec3 ShaderFunc::normalFragmentShader(const fragmentShaderPayload &payload)
{
	// normalize the normal then map [0,2] to [0,1]
	glm::vec3 nColor = (glm::normalize(payload.normal)) + glm::vec3(1.0, 1.0, 1.0);
	nColor /= 2.0;
	nColor *= 255.0;
	return nColor;
}


