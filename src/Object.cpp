#include "Object.h"
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
