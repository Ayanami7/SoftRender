#include "Render.h"
#include "tools.h"
#include "Shader.h"

Render::Render(int w, int h) : width(w), height(h)
{
	camera = nullptr;
	frameBuffer.resize(w * h);
    depthBuffer.resize(w * h);
    image_data = new unsigned char[w * h * 3];

	// create scene
	scene = new Scene;

	// default Shader func
	vertexShader = ShaderFunc::vertexShader;
	fragmentShader = ShaderFunc::normalFragmentShader;
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

void Render::drawWireframe()
{
	glm::vec3 color(150, 150, 150);
	if(camera == nullptr)
	{
		std::cout << "Error: Fail to load camera!" << std::endl;
		return;
	}

	this->clearBuffer(BufferType::COLOR_BUF);
	this->clearBuffer(BufferType::DEPTH_BUF);

	//mvp transform
	glm::mat4 viewMatrix = camera->getViewMatrix();
	glm::mat4 projectionMatrix = camera->getProjectionMatrix();

	auto objectLists = scene->allObjects();
	for (auto const &object : objectLists)
	{
		glm::mat4 modelMatrix = object.second->getModelMatirx();
		glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
		auto TriangleLists = object.second->getTriangleLists();
		for (auto const &t : TriangleLists)
		{
			// use to judge triangle whether need to be rendered
			bool flag = true;
			glm::vec4 v[] =
				{
					mvp * tovec4(t->vertex[0], 1.0f),
					mvp * tovec4(t->vertex[1], 1.0f),
					mvp * tovec4(t->vertex[2], 1.0f)};

			for (auto &vec : v)
			{
				vec.x /= vec.w;
				vec.y /= vec.w;
				vec.z /= vec.w;
			}

			// Map[-1,1] to [width,height];
			for (auto &vec : v)
			{
				vec.x = 0.5 * width * (vec.x + 1.0);
				vec.y = 0.5 * height * (vec.y + 1.0);

				// Map z to [zNear,zFar]
				float t1 = -(camera->getFar() + camera->getNear()) / 2;
				float t2 = -(camera->getFar() - camera->getNear()) / 2;
				vec.z = vec.z * t1 + t2;
			}

			for (int i = 0; i < 3; i++)
			{
				if (v[i].x < 0 || v[i].x >= width)
				{
					flag = false;
					break;
				}
				if (v[i].y < 0 || v[i].y >= height)
				{
					flag = false;
					break;
				}
			}

			if (flag)
			{
				drawLine(v[0], v[1], color);
				drawLine(v[1], v[2], color);
				drawLine(v[2], v[0], color);
			}
		}
	}
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
	if(vertexShader == nullptr)
	{
		std::cout << "Error: load vertexShader failed!" << std::endl;
		return;
	}
	if(fragmentShader == nullptr)
	{
		std::cout << "Error: load fragmentShader failed!" << std::endl;
		return;
	}
	if(camera == nullptr)
	{
		std::cout << "Error: load camera failed!" << std::endl;
		return;
	}
	// 
	this->clearBuffer(BufferType::COLOR_BUF);
	this->clearBuffer(BufferType::DEPTH_BUF);

	//mvp transform
	glm::mat4 viewMatrix = camera->getViewMatrix();
	glm::mat4 projectionMatrix = camera->getProjectionMatrix();

	auto objectLists = scene->allObjects();
	for (auto const &object : objectLists)
	{
		glm::mat4 modelMatrix = object.second->getModelMatirx();
		auto TriangleLists = object.second->getTriangleLists();
		for (auto const &t : TriangleLists)
		{
			glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
			Triangle triangle = *t;

			std::array<glm::vec4, 3> m
			{
				viewMatrix * modelMatrix * tovec4(t->vertex[0], 1.0f),
				viewMatrix * modelMatrix * tovec4(t->vertex[1], 1.0f),
				viewMatrix * modelMatrix * tovec4(t->vertex[2], 1.0f)
			};

			std::array<glm::vec3, 3> viewspace_pos;
			for (int i = 0; i < 3;i++)
			{
				viewspace_pos[i] = m[i];
			}

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
					inverseTrans * tovec4(t->normal[0], 0.0f),
					inverseTrans * tovec4(t->normal[1], 0.0f),
					inverseTrans * tovec4(t->normal[2], 0.0f)
				};

			// Map[-1,1] to [width,height];
			for(auto &vec : v)
			{
				vec.x = 0.5 * width * (vec.x + 1.0);
				vec.y = 0.5 * height * (vec.y + 1.0);
				// very very important
				/* I find my model's direction,normal texture and perspective relation
					all is not correct,because I am wrong when transform the z-value
					map the z-value from [-1,1] to [zear,zfar]
				*/
				float t1 = (camera->getFar() - camera->getNear()) / 2;
				float t2 = (camera->getFar() + camera->getNear()) / 2;
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

			rasterizeTriangle(triangle, viewspace_pos, object.second->getTexture());
		}
	}

}

void Render::rasterizeTriangle(const Triangle &t, const std::array<glm::vec3, 3> &view_pos,Texture *texture)
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
				// we only calculate the 2D pos
				// We need to multiple 1/w to restore the true z value
				// All in all, we need to use 2D x,y to interpolated calculate the z pos
				auto [a, b, c] = computeBarycentric2D(x + 0.5, y + 0.5, t.vertex);
                float z_interpolated = a * vex[0].z / vex[0].w + b * vex[1].z / vex[1].w + c * vex[2].z / vex[2].w;
				float w_reciprocal = 1.0 / (a / vex[0].w + b / vex[1].w + c / vex[2].w);
				z_interpolated *= w_reciprocal;

                if (z_interpolated < depthBuffer[getPos(x, y)])
                {
                    depthBuffer[getPos(x, y)] = z_interpolated;
					// glm::vec3 col(155, 155, 155);
					// frameBuffer[getPos(x, y)] = col;
					glm::vec3 color_i = a * t.vColor[0] + b * t.vColor[1] + c * t.vColor[2];
					glm::vec3 normal_i = glm::normalize(a * t.normal[0] + b * t.normal[1] + c * t.normal[2]);
					glm::vec2 texcoord_i = a * t.texCoord[0] + b * t.texCoord[1] + c * t.texCoord[2];
					glm::vec3 viewPos_i = a * view_pos[0] + b * view_pos[1] + c * view_pos[2];

					fragmentShaderPayload payload(color_i, normal_i, texcoord_i, texture);
					payload.viewPos = viewPos_i;
					payload.cameraPos = camera->eyePos();
					payload.ambLight = scene->getAmbLight();
					payload.lights = scene->allLights();
					auto pixel_color = fragmentShader(payload);
					frameBuffer[getPos(x, y)] = pixel_color;
				}
            }
        }
    }
}
