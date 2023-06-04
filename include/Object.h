#pragma once 
#include "global.h"
#include "Texture.h"
#include "Triangle.h"
#include <optional>

class Scene;
class Object
{
public:
	Object(const std::string &model_path);
	virtual ~Object();

	void loadTexture(const std::string &path)
	{
		texture = Texture(path);
	}
	Texture *getTexture()
	{
		return (texture.has_value() ? &*texture : nullptr);
	}
	std::vector<Triangle *> &getTriangleLists() { return TriangleLists; }
	glm::mat4 getModelMatirx();

private:
	std::vector<Triangle *> TriangleLists;		// vertex set
	std::optional<Texture> texture;
	float rotateAngle;
	glm::vec3 scale;
	glm::vec3 pos;
	glm::vec3 rotateAxis;

	//we can manage objects from the scene directly,but we lost some encapsulation
	friend class Scene;
};


