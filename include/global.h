/*
    define some global var
*/
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <functional>
#define MY_PI 3.14159

//sign the rotate axis
enum struct RotateAxis
{
	X_AXIS,
	Y_AXIS,
	Z_AXIS	
};

enum struct BufferType
{
	COLOR_BUF,
	DEPTH_BUF
};
