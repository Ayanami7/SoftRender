#include "Render.h"
#include "tools.h"

Render::Render(int w, int h) : width(w), height(h)
{
    frameBuffer.resize(w * h);
    depthBuffer.resize(w * h);
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

void Render::clearBuffer(BufferType type)
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

void Render::draw()
{
    this->clearBuffer(BufferType::DEPTH_BUF);
    for(auto t : TriangleLists)
    {
        rasterizeTriangle(*t);
    }
}


void Render::rasterizeTriangle(const Triangle &t)
{
    auto vex = t.tovec4(1.0f);
    //bounding Box边界	适当放大以防止边缘丢失
    int max_x = static_cast<int>(std::max(vex[0].x, std::max(vex[1].x, vex[2].x)) + 2);
    int min_x = static_cast<int>(std::min(vex[0].x, std::min(vex[1].x, vex[2].x)) - 2);
	int max_y = static_cast<int>(std::max(vex[0].y, std::max(vex[1].y, vex[2].y)) + 2);
	int min_y = static_cast<int>(std::min(vex[0].y, std::min(vex[1].y, vex[2].y)) - 2);

    for (int x = min_x; x < max_x; x++)
	{
		for (int y = min_y; y < max_y; y++)
		{
            //超出屏幕范围的数值直接丢弃,否则frameBuffer会越界
			if (x < 0 || x >= width || y < 0 || y >= height)
				continue;
            if (insideTriangle(x + 0.5, y + 0.5, t.vertex))
            {
                auto [a, b, c] = computeBarycentric2D(x + 0.5, y + 0.5, t.vertex);

                float z_interpolated = a * vex[0].z / vex[0].w + b * vex[1].z / vex[1].w + c * vex[2].z / vex[2].w;

                if (z_interpolated < depthBuffer[getPos(x, y)])
                {
                    glm::vec3 color = a * t.vColor[0] + b * t.vColor[1] + c * t.vColor[2];
                    frameBuffer[getPos(x, y)] = color;
                    depthBuffer[getPos(x, y)] = z_interpolated;
                }
            }
        }
    }
}
