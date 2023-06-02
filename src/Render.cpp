#include "Render.h"
#include "tools.h"
#include "Shader.h"

Render::Render(int w, int h) : width(w), height(h)
{
	camera = nullptr;
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
	for (int i = 0; i < 3;i++)
	{
		if(t.vertex[i].x <0||t.vertex[i].x>= width)
			return;
		if(t.vertex[i].y <0||t.vertex[i].y>= height)
			return;
	}
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
	if(camera == nullptr)
	{
		std::cout << "Error: Fail to load camera!" << std::endl;
		return;
	}

	this->clearBuffer(BufferType::DEPTH_BUF);

	//mvp transform
	glm::mat4 viewMatrix = camera->getViewMatrix();
	glm::mat4 projectionMatrix = camera->getProjectionMatrix();
	glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

    for(auto &t : TriangleLists)
    {
		// auto ver_vec4 = t->vec4Array(1.0);

		// //transform to camera coordinate system
		// ver_vec4[0] = viewMatrix * modelMatrix * ver_vec4[0];
		// ver_vec4[1] = viewMatrix * modelMatrix * ver_vec4[1];
		// ver_vec4[2] = viewMatrix * modelMatrix * ver_vec4[2];
		Triangle triangle = *t;
		glm::vec4 v[] =
			{
				mvp * tovec4(t->vertex[0], 1.0f),
				mvp * tovec4(t->vertex[1], 1.0f),
				mvp * tovec4(t->vertex[2], 1.0f)
			};

		for(auto &vec : v)
		{
			vec.x /= vec.w;
            vec.y /= vec.w;
            vec.z /= vec.w;
		}

		//use inverse transponse matrix to make sure the normal vec correctly
		glm::mat4 inverseTrans = glm::transpose(glm::inverse(viewMatrix * modelMatrix));

		glm::vec4 n[] =
			{
				inverseTrans * tovec4(t->normal[0], 1.0f),
				inverseTrans * tovec4(t->normal[1], 1.0f),
				inverseTrans * tovec4(t->normal[2], 1.0f)
			};

		// Map[-1,1] to [width,height];
		for(auto &vec : v)
		{
			vec.x = 0.5 * width * (vec.x + 1.0);
			vec.y = 0.5 * height * (vec.y + 1.0);
			float t1 = -(camera->getFar() + camera->getNear()) / 2;
			float t2 = -(camera->getFar() - camera->getNear()) / 2;
			vec.z = vec.z * t1 + t2;
		}

		for (int i = 0; i < 3;i++)
		{
			triangle.setVertex(i, glm::vec3(v[i].x, v[i].y, v[i].z));
		}

		for (int i = 0; i < 3;i++)
		{
			triangle.setNormal(i, glm::vec3(n[i].x, n[i].y, n[i].z));
		}

		glm::vec3 pen(100, 100, 100);
		drawWireframe(triangle, pen);
	}
}

void Render::rasterizeTriangle(const Triangle &t)
{
    auto vex = t.vec4Array(1.0f);
    //bounding Box
    int max_x = static_cast<int>(std::max(vex[0].x, std::max(vex[1].x, vex[2].x)) + 2);
    int min_x = static_cast<int>(std::min(vex[0].x, std::min(vex[1].x, vex[2].x)) - 2);
	int max_y = static_cast<int>(std::max(vex[0].y, std::max(vex[1].y, vex[2].y)) + 2);
	int min_y = static_cast<int>(std::min(vex[0].y, std::min(vex[1].y, vex[2].y)) - 2);

    for (int x = min_x; x < max_x; x++)
	{
		for (int y = min_y; y < max_y; y++)
		{
            //discard the data over the edge
			if (x < 0 || x >= width || y < 0 || y >= height)
				continue;
            if (insideTriangle(x + 0.5, y + 0.5, t.vertex))
            {
                auto [a, b, c] = computeBarycentric2D(x + 0.5, y + 0.5, t.vertex);

                float z_interpolated = a * vex[0].z / vex[0].w + b * vex[1].z / vex[1].w + c * vex[2].z / vex[2].w;

                if (z_interpolated < depthBuffer[getPos(x, y)])
                {
                    depthBuffer[getPos(x, y)] = z_interpolated;
                    glm::vec3 color_i = a * t.vColor[0] + b * t.vColor[1] + c * t.vColor[2];
					glm::vec3 normal_i = glm::normalize(a * t.normal[0] + b * t.normal[1] + c * t.normal[2]);
					glm::vec2 texcoord_i = a * t.texCoord[0] + b * t.texCoord[1] + c * t.texCoord[2];
					frameBuffer[getPos(x, y)] = color_i;
					
                }
            }
        }
    }
}
