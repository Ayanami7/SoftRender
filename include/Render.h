/*
    render the iamge
*/
#pragma once
#include "stb_image.h"
#include "stb_image_write.h"
#include "Triangle.h"
#include "Camera.h"
#include "Shader.h"
#include "Scene.h"
#include <functional>

struct HitRecord
{
	float tNear;
	uint32_t index;
	glm::vec2 uv;
	Object *obj;
};

class Render
{
public:
    Render(int w, int h);
    ~Render();

    inline int getPos(int x, int y) { return width * y + x; }   //through x, y get the frambuffer's pos
    void imagePrint(std::string path);                          //print image
    void drawLine(glm::vec3 startPos, glm::vec3 endPos, const glm::vec3 color);
    void drawWireframe();
    void rasterizeTriangle(const Triangle &t, const std::array<glm::vec3, 3> &view_pos,Texture *texture);
    void draw();

	// will set the framBuffer at [scene->backColor]
    void clearBuffer(BufferType type);

	void setScene(Scene *_scene) { scene = _scene; }
	void setCamera(Camera *pos) { camera = pos; }
	void setFragmentFunc(std::function<glm::vec3(fragmentShaderPayload)> &func) { fragmentShader = func; }

	Camera *getCamera() { return camera; }
	Scene *getScene() { return scene; }


	// ray tracing
	void render();
	glm::vec3 castRay(const Ray &ray, int depth);

private:
	Camera *camera;
	Scene *scene;			
	int height;
    int width;
	int recurveDepth = 5;		// control castRay recurve depth
	
	std::vector<float> depthBuffer;
    std::vector<glm::vec3> frameBuffer;		// save the color float 0-255 as RGB
    unsigned char *image_data;

	std::function<glm::vec3(vertexShaderPayload)> vertexShader;
	std::function<glm::vec3(fragmentShaderPayload)> fragmentShader;
};
