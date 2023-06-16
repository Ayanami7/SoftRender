#pragma once
#include "global.h"

class Camera
{
public:
	Camera(glm::vec3 _lookAt, glm::vec3 _lookFrom, glm::vec3 _up, float _near, float _far, float _fov, float _aspectRatio)
		: zNear(_near), zFar(_far), fov(_fov), aspectRatio(_aspectRatio)
	{
		auto theta = glm::radians(fov);
		auto h = tan(theta / 2.0);
		auto viewHeight = h * 2.0;
		auto viewWidth = viewHeight * _aspectRatio;

		//throgh cross vector to calculate the 3 axis of camera coordinate system
		//right hand coordinate system
		auto w = normalize(_lookFrom - _lookAt);
		auto u = normalize(glm::cross(_up, w));
		auto v = normalize(glm::cross(w, u));

		// create view and projection Matrix
		viewMatrix = glm::lookAtRH(_lookFrom, _lookAt, _up);
		projectionMatrix = glm::perspectiveRH(glm::radians(fov), _aspectRatio, _near, _far);

		origin = _lookFrom;
		horizontal = float(viewHeight) * u;
		vertical = float(viewWidth) * v;
		upperLeftCorner = origin - horizontal / 2.0f + vertical / 2.0f;
	}
	glm::mat4 getViewMatrix() { return viewMatrix; }
	glm::mat4 getProjectionMatrix() { return projectionMatrix; }
	glm::vec3 eyePos() { return origin; }
	inline float getNear() { return zNear; }
	inline float getFar() { return zFar; }
	inline float getRatio() { return aspectRatio; }
	inline float getFov() { return fov; }

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
