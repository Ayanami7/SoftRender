#pragma once
#include "global.h"
#include "Object.h"
#include <map>

class Scene
{
public:
	Scene();
	~Scene();
	void addObject(const std::string &model_name, const std::string &model_path);
	void deleteObject(const std::string &model_name);
	Object *getObject(const std::string &model_name);
	std::map<std::string, Object *> &allObjects() { return ObjectLists; }

	// change the status the object
	void rotateObject(Object *obj, const float &angle, const glm::vec3 &axis);
	void scaleObject(Object *obj, const glm::vec3 &ratio);
	void translateObject(Object *obj, const glm::vec3 &pos);

private:
	std::map<std::string, Object *> ObjectLists;
};
