#include "Object.h"
#include "tools.h"
#include "OBJ_Loader.h"

Object::Object(const std::string &model_path)
{
	//load the model
	objl::Loader Loader;

	bool isLoaded = Loader.LoadFile(model_path);
	if(!isLoaded)
	{
		throw std::runtime_error("Model loading error");
	}

	// Stores the meshes in the TriangleLists
	for(auto mesh : Loader.LoadedMeshes)
	{
		for (int i = 0; i < mesh.Vertices.size(); i += 3)
		{
			Triangle *t = new Triangle;
			// 3 vertexes
			for (int j = 0; j < 3; j++)
			{	
				// transform the 3D coordinate to the 4D coordinate
				t->setVertex(j, glm::vec4(mesh.Vertices[i + j].Position.X, mesh.Vertices[i + j].Position.Y, mesh.Vertices[i + j].Position.Z, 1.0f));
				t->setNormal(j, glm::vec3(mesh.Vertices[i + j].Normal.X, mesh.Vertices[i + j].Normal.Y, mesh.Vertices[i + j].Normal.Z));
				t->setTexCoord(j, glm::vec2(mesh.Vertices[i + j].TextureCoordinate.X, mesh.Vertices[i + j].TextureCoordinate.Y));
			}
			this->TriangleLists.push_back(t);
		}
	}
	// init pos and X axis
	material = new Material;		// set default material
	position = glm::vec3(0.0, 0.0, 0.0);
	rotateAxis = glm::vec3(0.0, 1.0, 0.0);
	scaleRatio = glm::vec3(1.0, 1.0, 1.0);
	rotateAngle = 0.0f;
}

Object::~Object()
{
	for(auto ptr : TriangleLists)
		delete ptr;
	TriangleLists.clear();
}

glm::mat4 Object::getModelMatirx()
{
	glm::mat4 res(1.0f);
	res = glm::translate(res, position);
	res = glm::rotate(res, glm::radians(rotateAngle), rotateAxis);
	res = glm::scale(res, scaleRatio);
	return res;
}

// process each triangle;
bool Object::intersect(const Ray &ray, float &tNear, uint32_t &index, glm::vec2& uv)
{
	bool intersect = false;
	for (uint32_t i = 0; i < TriangleLists.size();i++)
	{
		float u, v;
		float t;
		auto modelMat = getModelMatirx();
		glm::vec3 v0 = tovec3(modelMat * TriangleLists[i]->vertex[0]);
		glm::vec3 v1 = tovec3(modelMat * TriangleLists[i]->vertex[1]);
		glm::vec3 v2 = tovec3(modelMat * TriangleLists[i]->vertex[2]);
		if (rayTriangleIntersect(v0, v1, v2, ray, t, u, v) && t < tNear)
		{
			tNear = t;
			uv.x = u;
			uv.y = v;
			index = i;
			intersect |= true;
		}
	}
	return intersect;
}

void Object::getSurfaceProperty(const uint32_t &index, const glm::vec2 &uv, glm::vec3 &normal, glm::vec2 &st)
{
	const glm::vec3 v0 = tovec3(TriangleLists[index]->vertex[0]);
	const glm::vec3 v1 = tovec3(TriangleLists[index]->vertex[1]);
	const glm::vec3 v2 = tovec3(TriangleLists[index]->vertex[2]);

	glm::vec3 e0 = glm::normalize(v1 - v0);
	glm::vec3 e1 = glm::normalize(v2 - v1);

	normal = glm::normalize(glm::cross(e0, e1));
	const glm::vec2 st0 = TriangleLists[index]->texCoord[0];
	const glm::vec2 st1 = TriangleLists[index]->texCoord[1];
	const glm::vec2 st2 = TriangleLists[index]->texCoord[2];
	st = st0 * (1 - uv.x - uv.y) + st1 * uv.x + st2 * uv.y;
}
