#include "Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{
	for (auto obj : ObjectMap)
	{
		delete obj.second;
	}
	ObjectMap.clear();
	// destory the whole map
}

void Scene::addObject(const std::string &model_name, Object *obj)
{
	if(ObjectMap.count(model_name))
	{
		std::cout << "Error: can't add Object: " << model_name << " has existed!" << std::endl;
		return;
	}
	if (obj == nullptr)
	{
		std::cout << "Error: can't add Object: " << model_name << " not existed!" << std::endl;
		return;
	}
	ObjectMap[model_name] = obj;
}

void Scene::deleteObject(const std::string &model_name)
{
	// delete object and release memory
	if(!LightMap.count(model_name))
		std::cout << "Warnning: can't delete Object: " << model_name << " not exist!" << std::endl;
	delete ObjectMap.at(model_name);
	ObjectMap.erase(model_name);
}

Object *Scene::getObject(const std::string &model_name)
{
	if (!ObjectMap.count(model_name))
	{
		std::string err = "Error: can't find Object: " + model_name;
		throw std::runtime_error(err);
	}
	else
		return ObjectMap.at(model_name);
}

void Scene::rotateObject(Object *obj, const float &angle, const glm::vec3 &axis)
{
	obj->setRotateAngle(angle);
	obj->setRotateAxis(axis);
}

void Scene::scaleObject(Object *obj, const glm::vec3 &ratio)
{
	obj->setScaleRatio(ratio);
} 

void Scene::translateObject(Object *obj, const glm::vec3 &pos)
{
	obj->setPosition(pos);
}

void Scene::addLight(const std::string &light_name, const glm::vec3 &_position, const glm::vec3 &_intensity)
{
	if(LightMap.count(light_name))
	{
		std::cout << "Error: can't add Light: " << light_name << " has existed!" << std::endl;
	}
	Light *l = new Light(_position, _intensity);
	LightMap[light_name] = l;
}

void Scene::deleteLight(const std::string &light_name)
{
	// delete light and release memory
	if(!LightMap.count(light_name))
		std::cout << "Warnning: can't delete Light: " << light_name << " not exist!" << std::endl;
	delete LightMap.at(light_name);
	LightMap.erase(light_name);
}
