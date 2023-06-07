#include "Shader.h"
#include "Light.h"

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

glm::vec3 ShaderFunc::textureFragmentShader(const fragmentShaderPayload &payload)
{
	glm::vec3 returnColor(0, 0, 0);
	if (payload.texture)
    {
		returnColor = payload.texture->getColor(payload.tex_coords.x, payload.tex_coords.y);
	}
	glm::vec3 textureColor(returnColor);

	glm::vec3 ka = glm::vec3(0.005, 0.005, 0.005);
	glm::vec3 kd = textureColor / 255.0f;
	glm::vec3 ks = glm::vec3(0.7937, 0.7937, 0.7937);

	glm::vec3 cameraPos = payload.cameraPos;

	// hight light factor
	float p = 128;

	glm::vec3 color = textureColor;
	glm::vec3 point = payload.viewPos;
	glm::vec3 normal = payload.normal;

	glm::vec3 resultColor(0, 0, 0);

	auto lights = payload.lights;
	for(auto &light :lights)
	{
		glm::vec3 l = glm::normalize((light.second->position - point));	// light
		glm::vec3 v = glm::normalize((cameraPos - point));		// view dir
		glm::vec3 h = glm::normalize((l + v));

		float r_2 = glm::dot((light.second->position - point), (light.second->position - point)); 
		// use direction to replace intensity
		glm::vec3 ld = kd * (light.second->intensity / r_2) * std::max(0.0f, glm::dot(normal, l));
		glm::vec3 ls = ks * (light.second->intensity / r_2) * std::pow(std::max(0.0f, glm::dot(normal, h)), p);

		resultColor += (ld + ls);
	}

	glm::vec3 la = ka * (payload.ambLight);
	resultColor += la;
	resultColor *= 255.0f;
	return resultColor;
}


