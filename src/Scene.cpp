#include "Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::addObject(const std::string &model_name, const std::string &model_path)
{
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
