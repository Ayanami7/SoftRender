/*
    define some global var
*/
#pragma once
#include <glm/glm.hpp>
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