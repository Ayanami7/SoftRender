#include "Render.h"

Render::Render(int w, int h) : width(w), height(h)
{
    frameBuffer.resize(h * w);
    image_data = new unsigned char[w * h * 3];
}

Render::~Render()
{

}

void Render::drawLine(glm::vec3 startPos, glm::vec3 endPos, const glm::vec3 color)
{
    //use Bresenham’s Line Drawing Algorithm
    int x0 = static_cast<int>(std::floor(startPos.x));
    int y0 = static_cast<int>(std::floor(startPos.y));
    int x1 = static_cast<int>(std::floor(endPos.x));
    int y1 = static_cast<int>(std::floor(endPos.y));
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0; 
    int y = y0;
    for (int x = x0; x <= x1; x++)
    { 
        if (steep) 
            frameBuffer[getPos(y, x)] = color;
        else
            frameBuffer[getPos(x, y)] = color;
        error2 += derror2; 
        if (error2 > dx) 
        {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        } 
    } 
}

void Render::imagePrint(std::string path)
{
    //transform the buffer to iamge data
    for (size_t i = 0; i < frameBuffer.size();i++)
    {
        int x = i % width; // Calculates the position of pixels on the X-axis
        int y = static_cast<int>(height - 1 - i / width); // Calculates the position of pixels on the Y-axis

        // pixel save in png is per continue 3 unsigned char ragard as RGB
        image_data[getPos(x, y) * 3] = static_cast<unsigned char>(frameBuffer[i].x);
        image_data[getPos(x, y) * 3 + 1] = static_cast<unsigned char>(frameBuffer[i].y);
        image_data[getPos(x, y) * 3 + 2] = static_cast<unsigned char>(frameBuffer[i].z);
    }
    stbi_write_png(path.c_str(), width, height, 3, image_data, width * 3);
}

void Render::drawWireframe(Triangle &t, const glm::vec3 &color)
{
    drawLine(t.a(), t.b(), color);
    drawLine(t.b(), t.c(), color);
    drawLine(t.c(), t.a(), color);
}


void Render::buildBuffer()
{
    depthBuffer.resize(width * height);
    frameBuffer.resize(width * height);
}

void Render::clearBuffer(BufferType &type)
{
    if (type == BufferType::COLOR_BUF)
    {
        std::fill(frameBuffer.begin(), frameBuffer.end(), glm::vec3(0));
    }

    if (type == BufferType::DEPTH_BUF)
    {
        std::fill(depthBuffer.begin(), depthBuffer.end(), std::numeric_limits<float>::infinity());
    }
}