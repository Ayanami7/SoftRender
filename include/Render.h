/*
    render the iamge
*/
#pragma once
#include "global.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "Triangle.h"
#include "Camera.h"
#include <iostream>
#include <vector>


class Render
{
public:
    Render(int w, int h);
    ~Render();

    inline int getPos(int x, int y) { return width * y + x; }   //through x, y get the frambuffer's pos
    void imagePrint(std::string path);                          //print image
    void drawLine(glm::vec3 startPos, glm::vec3 endPos, const glm::vec3 color);
    void drawWireframe(Triangle &t, const glm::vec3 &color);
    void rasterizeTriangle(const Triangle &t);
    void draw();
    void clearBuffer(BufferType type);

    void setModelMatrix(const glm::mat4 &m) { modelMatrix = m; }
	void setCamera(Camera *pos) { camera = pos; }

	Camera *getCamera() { return camera; }

	std::vector<Triangle *> TriangleLists;
	int sum = 0;

private:
	Camera *camera;
	int height;
    int width;

    glm::mat4 modelMatrix;

    std::vector<float> depthBuffer;
    std::vector<glm::vec3> frameBuffer;
    unsigned char *image_data;

    void buildBuffer();
};
