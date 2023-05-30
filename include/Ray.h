#pragma once
#include "global.h"

class Ray
{
public:
    Ray() = default;
    Ray(const glm::vec3 &_origin, const glm::vec3 &_direction) : origin(_origin), direction(_direction){};
    glm::vec3 origin;
    glm::vec3 direction;
};
