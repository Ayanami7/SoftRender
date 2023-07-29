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
	scene = nullptr;

	// default Shader func
	vertexShader = ShaderFunc::vertexShader;
	fragmentShader = ShaderFunc::normalFragmentShader;
}

Render::~Render()
{
	delete image_data;
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
		// transform the zero point from the left-bottom to left-top
        int x = i % width; // Calculates the position of pixels on the X-axis
        int y = static_cast<int>(height - 1 - i / width); // Calculates the position of pixels on the Y-axis

        // pixel save in png is per continue 3 unsigned char ragard as RGB
		// when use function static_cast transform float to unsigned char
		// they will be overflow then the high light part of picture will produce a hole
		// we need to cut the float at 255 explicitly there
		if (frameBuffer[i].x >= 255.0f)
			image_data[getPos(x, y) * 3] = 255;
		else
			image_data[getPos(x, y) * 3] = static_cast<unsigned char>(frameBuffer[i].x);

		if (frameBuffer[i].y >= 255.0f)
			image_data[getPos(x, y) * 3 + 1] = 255;
		else
			image_data[getPos(x, y) * 3 + 1] = static_cast<unsigned char>(frameBuffer[i].y);

		if (frameBuffer[i].z >= 255.0f)
			image_data[getPos(x, y) * 3 + 2] = 255;
		else
        	image_data[getPos(x, y) * 3 + 2] = static_cast<unsigned char>(frameBuffer[i].z);
	}
	stbi_write_jpg(path.c_str(), width, height, 3, image_data, width * 3);
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
		glm::mat4 modelMatrix = object->getModelMatirx();
		glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
		auto TriangleLists = object->getTriangleLists();
		for (auto const &t : TriangleLists)
		{
			// use to judge triangle whether need to be rendered
			bool flag = true;
			glm::vec4 v[] =
				{
					mvp * t->vertex[0],
					mvp * t->vertex[1],
					mvp * t->vertex[2]
				};
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
		std::fill(frameBuffer.begin(), frameBuffer.end(), this->scene->getBackColor());
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
		glm::mat4 modelMatrix = object->getModelMatirx();
		auto TriangleLists = object->getTriangleLists();
		for (auto const &t : TriangleLists)
		{
			glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
			Triangle triangle = *t;

			std::array<glm::vec4, 3> m
			{
				viewMatrix * modelMatrix * t->vertex[0],
				viewMatrix * modelMatrix * t->vertex[1],
				viewMatrix * modelMatrix * t->vertex[2]
			};

			std::array<glm::vec3, 3> viewspace_pos;
			for (int i = 0; i < 3;i++)
			{
				viewspace_pos[i] = glm::vec3(m[i].x, m[i].y, m[i].z);
			}

			glm::vec4 v[] =
				{
					mvp * t->vertex[0],
					mvp * t->vertex[1],
					mvp * t->vertex[2]
				};

			for(auto &vec : v)
			{
				vec.x /= vec.w;
				vec.y /= vec.w;
				vec.z /= vec.w;
				// don't change the w to transfer the w-value to rasterizer
			}

			//use inverse transponse matrix to make sure the normal vec correctly
			glm::mat4 inverseTrans = glm::inverse(glm::transpose(viewMatrix * modelMatrix));

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
				triangle.setVertex(i, v[i]);
			}

			for (int i = 0; i < 3;i++)
			{
				triangle.setNormal(i, glm::vec3(n[i].x, n[i].y, n[i].z));
			}

			// set the color for default value to test phong shader
			glm::vec3 mColor = object->getMaterial()->mColor;
			triangle.setColor(0, mColor);
			triangle.setColor(1, mColor);
			triangle.setColor(2, mColor);

			rasterizeTriangle(triangle, viewspace_pos, object->getTexture());
		}
	}

}

void Render::rasterizeTriangle(const Triangle &t, const std::array<glm::vec3, 3> &view_pos,Texture *texture)
{
	auto vex = t.vec4Array();
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
				float numerator = a * vex[0].z / vex[0].w + b * vex[1].z / vex[1].w + c * vex[2].z / vex[2].w;
				float denominator = a / vex[0].w + b / vex[1].w + c / vex[2].w;
				float z_interpolated = numerator / denominator;
				if (z_interpolated < depthBuffer[getPos(x, y)])
                {
                    depthBuffer[getPos(x, y)] = z_interpolated;
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

//Limit the float in[lo,hi]
float clamp(const float &lo, const float &hi, const float &v)
{
	return std::max(lo, std::min(hi, v));
}

// calculate the reflect ray
glm::vec3 reflect(const glm::vec3 &in, const glm::vec3 &N)
{
	return in - 2 * glm::dot(in, N) * N;
}

// calculate the refract ray   ior = index of refraction
glm::vec3 refract(const glm::vec3 &in, const glm::vec3 &N, const float &ior)
{
	float cosi = clamp(-1, 1, glm::dot(in, N));
	float etai = 1;
	float etat = ior;
	glm::vec3 n = N;
	if(cosi < 0)
	{
		cosi = -cosi;
	}
	else
	{
		std::swap(etai, etat);
		n = -N;
	}
	float eta = etai / etat;
	float k = 1 - eta * eta * (1 - cosi * cosi);
	return k < 0 ? glm::vec3(0) : eta * in + (eta * cosi - sqrtf(k)) * n;
}

// calculate the fresnel index
float fresnel(const glm::vec3 &in, const glm::vec3 &N, const float &ior)
{
	float cosi = clamp(-1, 1, glm::dot(in, N));
	float etai = 1;
	float etat = ior;
	if (cosi > 0) 
	{
	std::swap(etai, etat);
	}
	// Compute sini using Snell's law
	float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
	// Total internal reflection
    if (sint >= 1) 
	{
        return 1;
    }
    else 
	{
		float cost = sqrtf(std::max(0.f, 1 - sint * sint));
		cosi = fabsf(cosi);
		float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
		return (Rs * Rs + Rp * Rp) / 2;
	}
}

HitRecord *trace(const Ray &ray, std::vector<Object *> &objects)
{
	// the min tNear
	float tNear = std::numeric_limits<float>::max();
	HitRecord *payload = nullptr;

	// traverse each Objects
	for (const auto &t : objects)
	{
		// throuh k+variant as now-value
		uint32_t indexK;
		glm::vec2 uvK;
		float tNearK = std::numeric_limits<float>::max();
		if (t->intersect(ray, tNearK, indexK, uvK) && tNearK < tNear)
		{
			payload = new HitRecord;
			payload->obj = t;
			payload->tNear = tNearK;
			payload->index = indexK;
			payload->uv = uvK;
			tNear = tNearK;		// updata the min values
		}
	}
	return payload;
}

glm::vec3 Render::castRay(const Ray &ray, int depth)
{
	// use depth to control recursion depth,max-value default as 5
	if (depth > this->recurveDepth)
		return glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 hitColor = scene->getBackColor();
	Scene *scene = getScene();
	if(auto payload = trace(ray,scene->allObjects()))
	{
		glm::vec3 hitPoint = ray.origin + ray.direction * payload->tNear;	//cross point
		glm::vec3 N;	//normal
		glm::vec2 st;
		payload->obj->getSurfaceProperty(payload->index, payload->uv, N, st);	// get N and st
		switch (payload->obj->getMaterial()->mType)
		{
			case REFLECTION_AND_REFRACTION:
			{
				// create the reflect Ray an refract Ray
				Ray reflectRay;
				reflectRay.direction = glm::normalize(reflect(ray.direction, N));
				reflectRay.origin = (glm::dot(reflectRay.direction, N) < 0) ? hitPoint - N * scene->getEpsilon() : hitPoint + N * scene->getEpsilon();
				Ray refractRay;
				refractRay.direction = glm::normalize(refract(ray.direction, N, payload->obj->getMaterial()->ior));
				refractRay.origin = (glm::dot(refractRay.direction, N) < 0) ? hitPoint - N * scene->getEpsilon() : hitPoint + N * scene->getEpsilon();
				//entry the recursion
				glm::vec3 reflectColor = castRay(reflectRay, depth + 1);
				glm::vec3 refractColor = castRay(refractRay, depth + 1);
				float kr = fresnel(ray.direction, N, payload->obj->getMaterial()->ior);
				hitColor = reflectColor * kr + refractColor * (1 - kr);
				break;
			}
			case REFLECTION:
			{
				float kr = fresnel(ray.direction, N, payload->obj->getMaterial()->ior);
				Ray reflectRay;
				reflectRay.direction = glm::normalize(reflect(ray.direction, N));
				reflectRay.origin = (glm::dot(reflectRay.direction, N) < 0 ? hitPoint - N * scene->getEpsilon() : hitPoint + N * scene->getEpsilon());
				hitColor = castRay(reflectRay, depth + 1) * kr;
				break;
			}
			default:
			{
				// default as phong model
				glm::vec3 lightAmt = glm::vec3(0);
				glm::vec3 specularColor = glm::vec3(0);
				glm::vec3 shadowPointOrigin = (glm::dot(ray.direction, N) < 0) ? hitPoint + N * scene->getEpsilon() : hitPoint - N * scene->getEpsilon();
				for (auto &light : scene->allLights())
				{
					glm::vec3 lightDir = light->position - hitPoint;
					float lightDistance2 = glm::dot(lightDir, lightDir);
					lightDir = glm::normalize(lightDir);
					float LdotN = std::max(0.0f, glm::dot(lightDir, N));
					Ray tmpRay(shadowPointOrigin, lightDir);
					auto shadow_res = trace(tmpRay, scene->allObjects());
					bool inShadow = shadow_res && (shadow_res->tNear * shadow_res->tNear < lightDistance2);

					lightAmt += inShadow ? glm::vec3(0) : light->intensity * LdotN;
					glm::vec3 reflectionDirection = reflect(-lightDir, N);

                    specularColor += powf(std::max(0.f, -glm::dot(reflectionDirection, ray.direction)),
                        payload->obj->getMaterial()->specularExponent) * light->intensity;
				}
				hitColor = lightAmt * payload->obj->getMaterial()->mColor * payload->obj->getMaterial()->Kd + specularColor * payload->obj->getMaterial()->Ks;
				break;
			}
		}
	}
	return hitColor;
}

// render the picture based on ray tracing 
void Render::render()
{
	glm::vec3 eyePos = camera->eyePos();
	float ratio = camera->getRatio();
	float theta = camera->getFov() * MY_PI / 180.0f;
	float scale = std::tan(theta / 2);
	Scene *scene = getScene();
	this->clearBuffer(BufferType::COLOR_BUF);
	this->clearBuffer(BufferType::DEPTH_BUF);
	float tNear = camera->getNear();
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			float x, y;
			x = (2 * (i + 0.5f) / height - 1) * scale * tNear * ratio;
			y = (2 * (j + 0.5f) / width - 1) * scale * tNear;
			y = -y;

			glm::vec3 dir = glm::vec3(x, y, -tNear) - eyePos;
			dir = glm::normalize(dir);
			Ray eyeRay(eyePos, dir);
			// entry the castRay
			auto mcolor = castRay(eyeRay, 0);
			this->frameBuffer[getPos(i, j)] = mcolor;
		}
		UpdateProgress(j / (float)height);
	}
	// save framebuffer to file
    FILE* fp = fopen("binary.ppm", "wb");
    (void)fprintf(fp, "P6\n%d %d\n255\n", width, height);
    for (auto i = 0; i < height * width; ++i) 
	{
        static unsigned char color[3];
        color[0] = (char)(frameBuffer[i].x);
        color[1] = (char)(frameBuffer[i].y);
        color[2] = (char)(frameBuffer[i].z);
        fwrite(color, 1, 3, fp);
    }
    fclose(fp);    
}
