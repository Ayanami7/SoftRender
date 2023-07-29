#include "Triangle.h"
#include "tools.h"

Triangle::Triangle(const glm::vec4 &v_a, const glm::vec4 &v_b, const glm::vec4 &v_c)
{
    vertex[0] = v_a;
    vertex[1] = v_b;
    vertex[2] = v_c;
}
Triangle::Triangle(const glm::vec4 &v_a, const glm::vec4 &v_b, const glm::vec4 &v_c,
                   const glm::vec3 &n_a, const glm::vec3 &n_b, const glm::vec3 &n_c)
{
    vertex[0] = v_a;
    vertex[1] = v_b;
    vertex[2] = v_c;
    normal[0] = n_a;
    normal[1] = n_b;
    normal[2] = n_c;
}

void Triangle::setVertex(int index, const glm::vec4 &v)
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

// transform the vec3(vertex[3]) to vec4 with w 
std::array<glm::vec4, 3> Triangle::vec4Array() const
{
	std::array<glm::vec4, 3> res =
		{
			vertex[0], vertex[1], vertex[2]
		};
	return res;
}

// Möller-Trumbore algorithm
bool rayTriangleIntersect(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2,
						  const Ray &ray, float &tnear, float &u, float &v)
{
	glm::vec3 v0v1 = v1 - v0;
	glm::vec3 v0v2 = v2 - v0;
	glm::vec3 N = glm::cross(v0v1, v0v2);
	float area2 = N.length();
	float NdotRayDir = glm::dot(N, ray.direction);

	if (fabs(NdotRayDir) < 0) // almost 0
        return false; // they are parallel, so they don't intersect!

	float d = glm::dot(N, v0);

	tnear = -(glm::dot(N, ray.origin) + d) / NdotRayDir;
	if (tnear < 0)
		return false;

	glm::vec3 P = ray.origin + tnear * ray.direction;

	glm::vec3 C;
	// edge 0
	glm::vec3 edge0 = v1 - v0; 
    glm::vec3 vp0 = P - v0;
	C = glm::cross(edge0, vp0);
	if (glm::dot(N, C) < 0)
		return false;

	// edge 1
    glm::vec3 edge1 = v2 - v1; 
    glm::vec3 vp1 = P - v1;
    C = glm::cross(edge1, vp1);
    if (glm::dot(N, C) < 0)
		return false; // P is on the right side

	// edge 2
    glm::vec3 edge2 = v0 - v2; 
    glm::vec3 vp2 = P - v2;
    C = glm::cross(edge2, vp2);
    if (glm::dot(N, C) < 0)
		return false; // P is on the right side;

	return true; // this ray hits the triangle
}

