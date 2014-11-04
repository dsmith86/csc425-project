#include "GLContext.h"
#include "vgl.h"

#pragma once

#define VERTEX_COUNT 36

typedef enum
{
	RED, GREEN, BLUE
} COLOR;

typedef vec3 color3;
typedef vec3 position3;

class Cube
{
public:
	Cube(position3 origin, vec3 scale, COLOR color);
	~Cube();
	model modelData();
private:
	position3 origin;
	vec3 scale;
	color3 color;
};

