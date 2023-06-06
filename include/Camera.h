#pragma once
#include "global.h"

class Camera
{
public:
	Camera(glm::vec3 _lookAt, glm::vec3 _lookFrom, glm::vec3 _up, float _near, float _far, float _fov, float _ratio);
	glm::mat4 getViewMatrix() { return viewMatrix; }
	glm::mat4 getProjectionMatrix() { return projectionMatrix; }
	glm::vec3 eyePos() { return origin; }
	inline float getNear() { return zNear; }
	inline float getFar() { return zFar; }

private:
	float zNear;					//positive value
	float zFar;
	float fov;
	float aspectRatio;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::vec3 origin;				//eye_pos
	glm::vec3 upperLeftCorner;
    glm::vec3 horizontal;			// horizontal range
    glm::vec3 vertical;				// vertical range
};
