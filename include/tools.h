#pragma once
#include "global.h"
#include "Triangle.h"
#include <tuple>

//calculate whether the pos are inside the triangle
bool insideTriangle(float x, float y, const glm::vec4 *vertex);

//calculate the areal coordinates
std::tuple<float, float, float> computeBarycentric2D(float x, float y, const glm::vec4 *v);

// vec3 with w-value to vec4
glm::vec4 tovec4(const glm::vec3 v, float w);

// vec4 to vec3
glm::vec3 tovec3(const glm::vec4 v);

// to solve the Quadratic equation  aX^2+bX+c = 0  X0 < X1
bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);
