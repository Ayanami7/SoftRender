/*
    render the iamge
*/
#pragma once
#include "global.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include "Triangle.h"
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
    void drawTriangle(Triangle &t);
    void clearBuffer(BufferType &type);

    void setModelMatrix(const glm::mat4 &m) { model = m; }
    void setViewMatrix(const glm::mat4 &v) { view = v; }
    void setProjectionMatrix(const glm::mat4 &p) { projection = p; }

private:
    int height;
    int width;

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    std::vector<Triangle *> TriangleLists;
    std::vector<float> depthBuffer;
    std::vector<glm::vec3> frameBuffer;
    unsigned char *image_data;

    void buildBuffer();
};