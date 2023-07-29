#pragma once 
#include "global.h"
#include "Texture.h"
#include "Material.h"
#include "Triangle.h"
#include <optional>

class Object
{
public:
	Object(const std::string &model_path);
	virtual ~Object();

	glm::mat4 getModelMatirx();

	bool intersect(const Ray &ray, float &tNear, uint32_t &index, glm::vec2 &uv);
	void getSurfaceProperty(const uint32_t index, const glm::vec2 uv, glm::vec3 &normal, glm::vec2 &st);

	inline virtual void loadTexture(const std::string &path)
	{
		texture = new Texture(path);
	}
	inline Texture *getTexture() { return texture; }
	inline void setMaterial(Material *m) { material = m; }
	inline Material *getMaterial() { return material; }

	inline std::vector<Triangle *> &getTriangleLists() { return TriangleLists; }

	inline void setRotateAngle(const float &angle) { rotateAngle = angle; }
	inline void setScaleRatio(const glm::vec3 &ratio) { scaleRatio = ratio; }
	inline void setPosition(const glm::vec3 &pos) { position = pos; }
	inline void setRotateAxis(const glm::vec3 &axis) { rotateAxis = axis; }

	inline float getRotateAngle() { return rotateAngle; }
	inline glm::vec3 getScaleRatio() { return scaleRatio; }
	inline glm::vec3 getPosition() { return position;}
	inline glm::vec3 getRotateAxis() { return rotateAxis; }

protected:
	std::vector<Triangle *> TriangleLists;		// vertex set
	Texture* texture;
	Material* material;
	float rotateAngle;
	glm::vec3 scaleRatio;
	glm::vec3 position;
	glm::vec3 rotateAxis;

	//we can manage objects from the scene directly,but we lost some encapsulation
};


