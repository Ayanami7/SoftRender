#pragma once
#include "global.h"
#include "Object.h"
#include "Model.h"
#include "Light.h"

class Scene
{
public:
	Scene();
	~Scene();
	// Object API
	void addObject(const std::string &model_name, Object *obj);
	void deleteObject(const std::string &model_name);
	Object *getObject(const std::string &model_name);
	// Point Light API
	void addLight(const std::string &light_name, const glm::vec3 &_position, const glm::vec3 &_intensity);
	void deleteLight(const std::string &light_name);
	// Ambient light setter/getter
	void setAmbLight(const glm::vec3 &_ambLight) { ambLight = _ambLight; }
	glm::vec3 &getAmbLight() { return ambLight; }
	// background color setter/getter
	void setBackColor(const glm::vec3 &_color) { backColor = _color; }
	glm::vec3 &getBackColor() { return backColor; }
	// set the min offset
	void setEpsilon(const float &f) { epsilon = f; }
	float &getEpsilon() { return epsilon; }
	std::vector<Object *> allObjects()
	{
		std::vector<Object *> ObjectLists;
		ObjectLists.reserve(ObjectMap.size());
		for (auto &item : ObjectMap)
		{
			ObjectLists.push_back(item.second);
		}
		return ObjectLists;
	}
	std::vector<Light *> allLights()
	{
		std::vector<Light *> LightLists;
		LightLists.reserve(ObjectMap.size());
		for (auto &item : LightMap)
		{
			LightLists.push_back(item.second);
		}
		return LightLists;
	}

	// change the status the object
	void rotateObject(Object *obj, const float &angle, const glm::vec3 &axis);
	void scaleObject(Object *obj, const glm::vec3 &ratio);
	void translateObject(Object *obj, const glm::vec3 &pos);

private:
	glm::vec3 backColor = glm::vec3(0, 0, 0);	// background color,default as black
	glm::vec3 ambLight = glm::vec3(0, 0, 0);	// Ambient light,default as 0
	std::map<std::string, Object *> ObjectMap;		//object list
	std::map<std::string, Light *> LightMap;		// point light list
	float epsilon = 0.00001;
};
