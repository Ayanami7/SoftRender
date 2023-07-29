#pragma once
#include "global.h"

enum MaterialType
{
	DIFFUSE_AND_GLOSSY,
	REFLECTION_AND_REFRACTION,
	REFLECTION
};

class Material
{
public:
	inline Material(MaterialType t = DIFFUSE_AND_GLOSSY, glm::vec3 c = glm::vec3(148.0, 121.0, 92.0), glm::vec3 e = glm::vec3(0.0, 0.0, 0.0));
	MaterialType mType;
	glm::vec3 mColor;
	glm::vec3 mEmission;
	float ior;		//index of refraction
	float Kd = 0.8;
	float Ks = 0.2;
	float specularExponent;

};

Material::Material(MaterialType t, glm::vec3 c, glm::vec3 e)
{
	mType = t;
    mColor = c;
    mEmission = e;
}
