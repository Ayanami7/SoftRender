#include "tools.h"

bool insideTriangle(float x, float y, const glm::vec3 *v)
{
    glm::vec3 point(x, y, 0.0);
    glm::vec3 edg1 = v[0] - v[1];
    glm::vec3 edg2 = v[1] - v[2];
    glm::vec3 edg3 = v[2] - v[0];

    glm::vec3 p1 = v[0] - point;
    glm::vec3 p2 = v[1] - point;
	glm::vec3 p3 = v[2] - point;

    float f1 = glm::cross(edg1, p1).z;
    float f2 = glm::cross(edg2, p2).z;
    float f3 = glm::cross(edg3, p3).z;

    bool flag = false;
    if ((f1 > 0 && f2 > 0 && f3 > 0) ||
        (f1 < 0 && f2 < 0 && f3 < 0))
        flag = true;
    return flag;
}

std::tuple<float, float, float> computeBarycentric2D(float x, float y, const glm::vec3* v)
{
    float c1 = (x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * y + v[1].x * v[2].y - v[2].x * v[1].y) / (v[0].x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * v[0].y + v[1].x * v[2].y - v[2].x * v[1].y);
    float c2 = (x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * y + v[2].x * v[0].y - v[0].x * v[2].y) / (v[1].x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * v[1].y + v[2].x * v[0].y - v[0].x * v[2].y);
    float c3 = (x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * y + v[0].x * v[1].y - v[1].x * v[0].y) / (v[2].x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * v[2].y + v[0].x * v[1].y - v[1].x * v[0].y);
    return {c1, c2, c3};
}

glm::vec4 tovec4(glm::vec3 v, float w)
{
	glm::vec4 res(v.x, v.y, v.z, w);
	return std::move(res);
}
