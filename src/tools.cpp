#include "tools.h"

bool insideTriangle(float x, float y, const glm::vec4 *vertex)
{
    glm::vec3 point(x, y, 0.0);

	glm::vec3 v0 = tovec3(vertex[0]);
	glm::vec3 v1 = tovec3(vertex[1]);
	glm::vec3 v2 = tovec3(vertex[2]);

	glm::vec3 edg1 = v0 - v1;
    glm::vec3 edg2 = v1 - v2;
    glm::vec3 edg3 = v2 - v0;

    glm::vec3 p1 = v0 - point;
    glm::vec3 p2 = v1 - point;
	glm::vec3 p3 = v2 - point;

    float f1 = glm::cross(edg1, p1).z;
    float f2 = glm::cross(edg2, p2).z;
    float f3 = glm::cross(edg3, p3).z;

    bool flag = false;
    if ((f1 > 0 && f2 > 0 && f3 > 0) ||
        (f1 < 0 && f2 < 0 && f3 < 0))
        flag = true;
    return flag;
}

std::tuple<float, float, float> computeBarycentric2D(float x, float y, const glm::vec4* v)
{
    float c1 = (x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * y + v[1].x * v[2].y - v[2].x * v[1].y) / (v[0].x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * v[0].y + v[1].x * v[2].y - v[2].x * v[1].y);
    float c2 = (x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * y + v[2].x * v[0].y - v[0].x * v[2].y) / (v[1].x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * v[1].y + v[2].x * v[0].y - v[0].x * v[2].y);
    float c3 = (x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * y + v[0].x * v[1].y - v[1].x * v[0].y) / (v[2].x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * v[2].y + v[0].x * v[1].y - v[1].x * v[0].y);
    return {c1, c2, c3};
}

glm::vec4 tovec4(const glm::vec3 v, float w)
{
	glm::vec4 res(v.x, v.y, v.z, w);
	return std::move(res);
}

glm::vec3 tovec3(const glm::vec4 v)
{
	glm::vec3 res(v.x, v.y, v.z);
	return std::move(res);
}

bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
{
    float discr = b * b - 4 * a * c;
    if (discr < 0) return false;
    else if (discr == 0) x0 = x1 = - 0.5 * b / a;
    else {
        float q = (b > 0) ?
                  -0.5 * (b + sqrt(discr)) :
                  -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);
    return true;
}
