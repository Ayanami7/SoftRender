/*
    struct of triangle
*/
#pragma once
#include "global.h"
#include "Ray.h"
#include <array>

class Triangle
{
public:
    glm::vec4 vertex[3];         // vertex
    glm::vec3 vColor[3];    // vertex color
    glm::vec2 texCoord[3]; // texture coord
    glm::vec3 normal[3];    // normal vector

	Triangle(){};
	Triangle(const glm::vec4 &v_a, const glm::vec4 &v_b, const glm::vec4 &v_c);
    Triangle(const glm::vec4 &v_a, const glm::vec4 &v_b, const glm::vec4 &v_c,
             const glm::vec3 &n_a, const glm::vec3 &n_b, const glm::vec3 &n_c);
    glm::vec4 a() { return vertex[0]; }
    glm::vec4 b() { return vertex[1]; }
    glm::vec4 c() { return vertex[2]; }

    void setVertex(int index, const glm::vec4 &v);
    void setNormal(int index, const glm::vec3 &n);
    void setColor(int index, const glm::vec3 &v_color);
    void setTexCoord(int index, const glm::vec2 &t_coord);

	std::array<glm::vec4, 3> vec4Array() const;
};

bool rayTriangleIntersect(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2,
						  const Ray &ray, float &tnear, float &u, float &v);
