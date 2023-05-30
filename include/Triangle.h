/*
    struct of triangle
*/
#pragma once
#include "global.h"
#include <array>

class Triangle
{
public:
    glm::vec3 vertex[3];         // vertex
    glm::vec3 vColor[3];    // vertex color
    glm::vec2 texCoord[3]; // texture coord
    glm::vec3 normal[3];    // normal vector

    Triangle(const glm::vec3 &v_a, const glm::vec3 &v_b, const glm::vec3 &v_c);
    Triangle(const glm::vec3 &v_a, const glm::vec3 &v_b, const glm::vec3 &v_c,
             const glm::vec3 &n_a, const glm::vec3 &n_b, const glm::vec3 &n_c);
    glm::vec3 a() { return vertex[0]; }
    glm::vec3 b() { return vertex[1]; }
    glm::vec3 c() { return vertex[2]; }

    void setVertex(int index, const glm::vec3 &v);
    void setNormal(int index, const glm::vec3 &n);
    void setColor(int index, const glm::vec3 &v_color);
    void setTexCoord(int index, const glm::vec2 &t_coord);

    std::array<glm::vec4, 3> tovec4(float w) const;
};