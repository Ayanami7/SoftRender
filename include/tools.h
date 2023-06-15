#pragma once
#include "global.h"
#include "Triangle.h"
#include <tuple>

//calculate whether the pos are inside the triangle
bool insideTriangle(float x, float y, const glm::vec4 *vertex);

//calculate the areal coordinates
std::tuple<float, float, float> computeBarycentric2D(float x, float y, const glm::vec4 *v);

glm::vec4 tovec4(const glm::vec3 v, float w);

glm::vec3 tovec3(const glm::vec4 v);
