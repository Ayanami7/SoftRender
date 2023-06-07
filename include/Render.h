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
    void clearBuffer(BufferType type);

	void setCamera(Camera *pos) { camera = pos; }
	void setFragmentFunc(std::function<glm::vec3(fragmentShaderPayload)> &func) { fragmentShader = func; }

	Camera *getCamera() { return camera; }
	Scene *getScene() { return scene; }


private:
	Camera *camera;			//camera object
	Scene *scene;
	int height;
    int width;

    std::vector<float> depthBuffer;
    std::vector<glm::vec3> frameBuffer;
    unsigned char *image_data;

	std::function<glm::vec3(vertexShaderPayload)> vertexShader;
	std::function<glm::vec3(fragmentShaderPayload)> fragmentShader;
};
