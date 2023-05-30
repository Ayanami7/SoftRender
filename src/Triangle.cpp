#include "Triangle.h"

Triangle::Triangle(const glm::vec3 &v_a, const glm::vec3 &v_b, const glm::vec3 &v_c)
{
    vertex[0] = v_a;
    vertex[1] = v_b;
    vertex[2] = v_c;
}
Triangle::Triangle(const glm::vec3 &v_a, const glm::vec3 &v_b, const glm::vec3 &v_c,
                   const glm::vec3 &n_a, const glm::vec3 &n_b, const glm::vec3 &n_c)
{
    vertex[0] = v_a;
    vertex[1] = v_b;
    vertex[2] = v_c;
    normal[0] = n_a;
    normal[1] = n_b;
    normal[2] = n_c;
}

void Triangle::setVertex(int index, const glm::vec3 &v)
{
    vertex[index] = v;
}

void Triangle::setNormal(int index, const glm::vec3 &n)
{
    normal[index] = n;
}

void Triangle::setColor(int index, const glm::vec3 &v_color)
{
    vColor[index] = v_color;
}

void Triangle::setTexCoord(int index, const glm::vec2 &tex_coord)
{
    texCoord[index] = tex_coord;
}