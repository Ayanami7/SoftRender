#pragma once
#include "global.h"
#include "Ray.h"
class Camera
{
public:

private:
    glm::vec3 origin;
    glm::vec3 upperLeftCorner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
};