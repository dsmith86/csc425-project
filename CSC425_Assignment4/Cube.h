#include "GLContext.h"
#include "vgl.h"

#pragma once

#define VERTEX_COUNT 36
#define FACE_COUNT 6

typedef enum
{
	RED, GREEN, BLUE
} COLOR;

typedef enum
{
	CUBE_DIRECTION_UP,
	CUBE_DIRECTION_DOWN,
	CUBE_DIRECTION_LEFT,
	CUBE_DIRECTION_RIGHT,
	CUBE_DIRECTION_FRONT,
	CUBE_DIRECTION_BACK
} DIRECTIONS;

typedef GLContext::vec3 color3;
typedef GLContext::vec3 scale3;
typedef GLContext::vec3 position3;
typedef GLContext::vec3 rotation3;

class Cube
{
public:
	Cube(position3 position, scale3 scale, COLOR color, const char *shader); // basic, shaded
	Cube(position3 position, scale3 scale, const char *shader, float texCoord0, float texCoord1); // textured
	Cube* setRotation(GLContext::DIRECTION direction, float theta);
	~Cube();
	GLContext::model modelData();
private:
	position3 position;
	scale3 scale;
	GLContext::DIRECTION rotationAxis;
	float rotationTheta;
	color3 color;
	const char *shader;
	GLenum renderType;
	float texCoord0, texCoord1;
};

