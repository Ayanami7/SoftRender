#pragma once
#include "global.h"

// point light
class Light
{
public:
	Light(const glm::vec3 &_position, const glm::vec3 &_intensity)
		: position(_position), intensity(_intensity){}
	virtual ~Light(){}
	glm::vec3 position;
    glm::vec3 intensity;
};
