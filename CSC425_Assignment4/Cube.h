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
typedef GLContext::vec3 position3;

class Cube
{
public:
	Cube(position3 position, GLContext::vec3 scale, COLOR color, const char *shader, GLfloat shininess);
	~Cube();
	GLContext::model modelData();
private:
	position3 position;
	GLContext::vec3 scale;
	color3 color;
	const char *shader;
	GLenum renderType;
	GLfloat shininess;
};

