#include "Shader.h"
#include "Ray.h"

glm::vec3 ShaderFunc::vertexShader(const vertexShaderPayload &payload)
{
	return payload.position;
}


glm::vec3 ShaderFunc::normalFragmentShader(const fragmentShaderPayload &payload, const glm::vec3& eye_pos)
{
	// normalize the normal then map [0,2] to [0,1]
	glm::vec3 nColor = (glm::normalize(payload.normal)) + glm::vec3(1.0, 1.0, 1.0);
	nColor /= 2.0;
	nColor *= 255.0;
	return nColor;
}

glm::vec3 ShaderFunc::textureFragmentShader(const fragmentShaderPayload &payload, const glm::vec3& eye_pos)
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

	// define two point light to test
	auto l1 = Ray{{20, 20, 20}, {500, 500, 500}};
    auto l2 = Ray{{-20, 20, 20}, {500, 500, 500}};

	std::vector<Ray> lights = {l1, l2};
    glm::vec3 amb_light_intensity{10, 10, 10};

	float p = 128;

	glm::vec3 color = textureColor;
	glm::vec3 point = payload.viewPos;
	glm::vec3 normal = payload.normal;

	glm::vec3 resultColor(0, 0, 0);

	for(auto &light :lights)
	{
		glm::vec3 l = glm::normalize((light.origin - point));	// light
		glm::vec3 v = glm::normalize((eye_pos - point));		// view dir
		glm::vec3 h = glm::normalize((l + v));

		float r_2 = glm::dot((light.origin - point), (light.origin - point)); 
		// use direction to replace intensity
		glm::vec3 ld = kd * (light.direction / r_2) * std::max(0.0f, glm::dot(normal, l));
		glm::vec3 ls = ks * (light.direction / r_2) * std::pow(std::max(0.0f, glm::dot(normal, h)), p);

		resultColor += (ld + ls);
	}

	glm::vec3 la = ka * (amb_light_intensity);
	resultColor += la;
	resultColor *= 255.0f;
	return resultColor;
}


