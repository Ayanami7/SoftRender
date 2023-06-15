#pragma once 
#include "global.h"
#include "Texture.h"
#include "Triangle.h"
#include <optional>

class Object
{
public:
	Object(const std::string &model_path);
	virtual ~Object();

	Texture *getTexture()
	{
		return (texture.has_value() ? &*texture : nullptr);
	}
	std::vector<Triangle *> &getTriangleLists() { return TriangleLists; }
	glm::mat4 getModelMatirx();
	virtual void loadTexture(const std::string &path)
	{
		texture = Texture(path);
	}

	void setRotateAngle(const float angle) { rotateAngle = angle; }
	void setScaleRatio(const glm::vec3 ratio) { scaleRatio = ratio; }
	void setPosition(const glm::vec3 pos) { position = pos; }
	void setRotateAxis(const glm::vec3 axis) { rotateAxis = axis; }

	float getRotateAngle() { return rotateAngle; }
	glm::vec3 getScaleRatio() { return scaleRatio; }
	glm::vec3 getPosition() { return position;}
	glm::vec3 getRotateAxis() { return rotateAxis; }

protected:
	std::vector<Triangle *> TriangleLists;		// vertex set
	std::optional<Texture> texture;
	float rotateAngle;
	glm::vec3 scaleRatio;
	glm::vec3 position;
	glm::vec3 rotateAxis;

	//we can manage objects from the scene directly,but we lost some encapsulation
};


