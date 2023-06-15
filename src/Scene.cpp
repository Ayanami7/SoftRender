#include "Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{
	for (auto obj : ObjectLists)
	{
		delete obj.second;
	}
	ObjectLists.clear();
	// destory the whole map
}

void Scene::addObject(const std::string &model_name, Object *obj)
{
	if(ObjectLists.count(model_name))
	{
		std::cout << "Error: can't add Object: " << model_name << " has existed!" << std::endl;
		return;
	}
	if (obj == nullptr)
	{
		std::cout << "Error: can't add Object: " << model_name << " not existed!" << std::endl;
		return;
	}
	ObjectLists[model_name] = obj;
}

void Scene::deleteObject(const std::string &model_name)
{
	// delete object and release memory
	if(!LightLists.count(model_name))
		std::cout << "Warnning: can't delete Object: " << model_name << " not exist!" << std::endl;
	delete ObjectLists.at(model_name);
	ObjectLists.erase(model_name);
}

Object *Scene::getObject(const std::string &model_name)
{
	if (!ObjectLists.count(model_name))
	{
		std::string err = "Error: can't find Object: " + model_name;
		throw std::runtime_error(err);
	}
	else
		return ObjectLists.at(model_name);
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
	if(LightLists.count(light_name))
	{
		std::cout << "Error: can't add Light: " << light_name << " has existed!" << std::endl;
	}
	Light *l = new Light(_position, _intensity);
	LightLists[light_name] = l;
}

void Scene::deleteLight(const std::string &light_name)
{
	// delete light and release memory
	if(!LightLists.count(light_name))
		std::cout << "Warnning: can't delete Light: " << light_name << " not exist!" << std::endl;
	delete LightLists.at(light_name);
	LightLists.erase(light_name);
}
