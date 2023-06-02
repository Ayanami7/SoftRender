/*
    render the iamge
*/
#pragma once
#include "global.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "Triangle.h"
#include "Camera.h"
#include "Shader.h"
#include <optional>

class Render
{
public:
    Render(int w, int h);
    ~Render();

    inline int getPos(int x, int y) { return width * y + x; }   //through x, y get the frambuffer's pos
    void imagePrint(std::string path);                          //print image
    void drawLine(glm::vec3 startPos, glm::vec3 endPos, const glm::vec3 color);
    void drawWireframe();
    void rasterizeTriangle(const Triangle &t);
    void draw();
    void clearBuffer(BufferType type);

    void setModelMatrix(const glm::mat4 &m) { modelMatrix = m; }
	void setCamera(Camera *pos) { camera = pos; }
	void setTexure(Texture *tex) { texture = tex; }

	Camera *getCamera() { return camera; }

	std::vector<Triangle *> TriangleLists;

private:
	Camera *camera;			//camera object
	int height;
    int width;

    glm::mat4 modelMatrix;	//model matrix    then willbe replace to scene object

    std::vector<float> depthBuffer;
    std::vector<glm::vec3> frameBuffer;
    unsigned char *image_data;

	std::function<glm::vec3(vertexShaderPayload)> vertexShader;
	std::function<glm::vec3(fragmentShaderPayload)> fragmentShader;
	Texture *texture;

	void buildBuffer();
};
