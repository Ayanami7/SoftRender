#include "Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::addObject(const std::string &model_name, const std::string &model_path)
{
	if(ObjectLists.count(model_name))
	{
		std::cout << "Error: can't add Object: " << model_name << " has existed!" << std::endl;
		return;
	}
	try
	{
		Object *obj = new Object(model_path);
		ObjectLists[model_name] = obj;
	}
	catch(const std::runtime_error &e)
	{
		std::cerr << e.what() << "  Model Name: " << model_name << std::endl;
		return;
	}
	

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
	obj->rotateAngle = angle;
	obj->rotateAxis = axis;
}

void Scene::scaleObject(Object *obj, const glm::vec3 &ratio)
{
	obj->scaleRatio = ratio;
} 

void Scene::translateObject(Object *obj, const glm::vec3 &pos)
{
	obj->pos = pos;
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
