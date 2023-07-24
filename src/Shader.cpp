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

	// high light factor
	float p = 128;

	glm::vec3 color = textureColor / 255.0f;
	glm::vec3 point = payload.viewPos;
	glm::vec3 normal = payload.normal;

	glm::vec3 resultColor(0, 0, 0);
	auto lights = payload.lights;
	for(auto &light :lights)
	{
		glm::vec3 l = glm::normalize((light->position - point));	// light
		glm::vec3 v = glm::normalize((cameraPos - point));		// view dir
		glm::vec3 h = glm::normalize((l + v));
		float r_2 = glm::dot((light->position - point), (light->position - point));
		glm::vec3 t = light->intensity / r_2;
		glm::vec3 ld = glm::vec3(kd.x * t.x, kd.y * t.y, kd.z * t.z) * std::max(0.0f, glm::dot(normal, l));
		glm::vec3 ls = glm::vec3(ks.x * t.x, ks.y * t.y, ks.z * t.z) * std::pow(std::max(0.0f, glm::dot(normal, h)), p);

		resultColor += (ld + ls);
	}

	glm::vec3 la = ka * (payload.ambLight);
	resultColor += la;
	resultColor *= 255.0f;
	return resultColor;
}

glm::vec3 ShaderFunc::phongFragmentShader(const fragmentShaderPayload &payload)
{
	glm::vec3 ka = glm::vec3(0.005, 0.005, 0.005);
	glm::vec3 kd = payload.color / 255.0f;		//there use 0-255 float as RGB
	glm::vec3 ks = glm::vec3(0.7937, 0.7937, 0.7937);

	glm::vec3 cameraPos = payload.cameraPos;

	// high light factor
	float p = 128;

	glm::vec3 color = payload.color / 255.0f;
	glm::vec3 point = payload.viewPos;
	glm::vec3 normal = payload.normal;

	glm::vec3 resultColor(0, 0, 0);

	auto lights = payload.lights;
	for(auto &light :lights)
	{
		glm::vec3 l = glm::normalize((light->position - point));	// light
		glm::vec3 v = glm::normalize((cameraPos - point));		// view dir
		glm::vec3 h = glm::normalize((l + v));

		float r_2 = glm::dot((light->position - point), (light->position - point)); 
		glm::vec3 t = light->intensity / r_2;
		glm::vec3 ld = glm::vec3(kd.x * t.x, kd.y * t.y, kd.z * t.z) * std::max(0.0f, glm::dot(normal, l));
		glm::vec3 ls = glm::vec3(ks.x * t.x, ks.y * t.y, ks.z * t.z) * std::pow(std::max(0.0f, glm::dot(normal, h)), p);

		resultColor += (ld + ls);
	}

	glm::vec3 la = ka * (payload.ambLight);
	resultColor += la;
	resultColor *= 255.0f;
	return resultColor;
}

glm::vec3 ShaderFunc::bumpFragmentShader(const fragmentShaderPayload &payload)
{
	glm::vec3 ka = glm::vec3(0.005, 0.005, 0.005);
    glm::vec3 kd = payload.color /255.0f;
    glm::vec3 ks = glm::vec3(0.7937, 0.7937, 0.7937);

	auto lights = payload.lights;

	float p = 150;

	glm::vec3 color = payload.color / 255.0f;
	glm::vec3 point = payload.viewPos;
	glm::vec3 normal = payload.normal;

	float kh = 0.2f, kn = 0.1f;

	float x = normal.x;
	float y = normal.y;
	float z = normal.z;
	glm::vec3 t = glm::normalize(glm::vec3(x * y / std::sqrt(x * x + z * z), std::sqrt(x * x + z * z), z * y / std::sqrt(x * x + z * z)));
	glm::vec3 b = glm::normalize(glm::cross(normal, t));

	glm::mat3 TBN(t, b, normal);

	float u = payload.tex_coords.x;
	float v = payload.tex_coords.y;
	float w = payload.texture->width;
	float h = payload.texture->height;

	glm::vec3 cameraPos = payload.cameraPos;

	float dU = kh * kn * (glm::length(payload.texture->getColor(u + 1.0f / w, v)) - glm::length(payload.texture->getColor(u, v)));
	float dV = kh * kn * (glm::length(payload.texture->getColor(u, v + 1.0f / h)) - glm::length(payload.texture->getColor(u, v)));
	glm::vec3  ln{-dU, -dV, 1};

	point += (kn * normal * glm::length(payload.texture->getColor(u, v)));
	normal = glm::normalize(TBN * ln);

	return normal * 255.0f;
}

//displacement shader
glm::vec3 ShaderFunc::displacementFragmentShader(const fragmentShaderPayload &payload)
{
	glm::vec3 ka = glm::vec3(0.005, 0.005, 0.005);
    glm::vec3 kd = payload.color /255.0f;
    glm::vec3 ks = glm::vec3(0.7937, 0.7937, 0.7937);

	auto lights = payload.lights;

	float p = 128;

	glm::vec3 color = payload.color / 255.0f;
	glm::vec3 point = payload.viewPos;
	glm::vec3 normal = payload.normal;

	float kh = 0.2f, kn = 0.1f;

	float x = normal.x;
	float y = normal.y;
	float z = normal.z;
	glm::vec3 t = glm::normalize(glm::vec3(x * y / std::sqrt(x * x + z * z), std::sqrt(x * x + z * z), z * y / std::sqrt(x * x + z * z)));
	glm::vec3 b = glm::normalize(glm::cross(normal, t));

	glm::mat3 TBN(t, b, normal);

	float u = payload.tex_coords.x;
	float v = payload.tex_coords.y;
	float w = payload.texture->width;
	float h = payload.texture->height;

	glm::vec3 cameraPos = payload.cameraPos;

	float dU = kh * kn * (glm::length(payload.texture->getColor(u + 1.0f / w, v)) - glm::length(payload.texture->getColor(u, v)));
	float dV = kh * kn * (glm::length(payload.texture->getColor(u, v + 1.0f / h)) - glm::length(payload.texture->getColor(u, v)));
	glm::vec3  ln{-dU, -dV, 1};

	point += (kn * normal * glm::length(payload.texture->getColor(u, v)));
	normal = glm::normalize(TBN * ln);
	glm::vec3 resultColor = {0, 0, 0};
	for(auto &light :lights)
	{
		glm::vec3 l = glm::normalize((light->position - point));	// light
		glm::vec3 v = glm::normalize((cameraPos - point));		// view dir
		glm::vec3 h = glm::normalize((l + v));

		float r_2 = glm::dot((light->position - point), (light->position - point));

		glm::vec3 ld = kd * (light->intensity / r_2) * std::max(0.0f, glm::dot(normal, l));
		glm::vec3 ls = ks * (light->intensity / r_2) * std::pow(std::max(0.0f, glm::dot(normal, h)), p);

		resultColor += (ld + ls);
	}
	glm::vec3 la = ka * payload.ambLight;
	resultColor += la;
	resultColor *= 255.0f;
	return resultColor;
}


