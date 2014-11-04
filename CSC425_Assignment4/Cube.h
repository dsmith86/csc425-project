#include "GLContext.h"
#include "vgl.h"

#pragma once

#define VERTEX_COUNT 14

typedef enum
{
	RED, GREEN, BLUE
} COLOR;

typedef vec3 color3;
typedef vec3 position3;

class Cube
{
public:
	Cube(position3 origin, vec3 scale, COLOR color, const char *shader);
	~Cube();
	model modelData();
private:
	position3 origin;
	vec3 scale;
	color3 color;
	const char *shader;
	GLenum renderType;
};

