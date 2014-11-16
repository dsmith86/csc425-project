#include "Cube.h"
#include <iostream>
using namespace std;

void pushBack(vector<GLfloat> *vector, int times, DIRECTIONS d, int arraySize);

Cube::Cube(position3 position, GLContext::vec3 scale, COLOR color, const char *shader, GLfloat shininess)
{
	this->position = position;
	this->scale = scale;
	this->shader = shader;
	this->renderType = renderType;
	this->shininess = shininess;
	
	switch (color)
	{
	case COLOR::RED:
		this->color = { 1.0, 0.0, 0.0 };
		break;
	case COLOR::GREEN:
		this->color = { 0.0, 1.0, 0.0 };
		break;
	case COLOR::BLUE:
		this->color = { 0.0, 0.0, 1.0 };
		break;
	default:
		this->color = { 1.0, 1.0, 1.0 };
		break;
	}
}


Cube::~Cube()
{
}

GLContext::model Cube::modelData()
{
	GLContext::model m;

	float L = this->scale.x * 0.5f;
	float U = this->scale.y * 0.5f;
	float B = this->scale.z * 0.5f;
	float R = -L;
	float D = -U;
	float F = -B;

	// courtesy of research publication "Optimizing Triangle Strips for Fast Rendering"
	// http://www.cs.umd.edu/gvil/papers/av_ts.pdf

	GLfloat vertices[VERTEX_COUNT][VECTOR_SIZE] = {
			{ R, U, F }, { R, U, B }, { L, U, B }, // UP
			{ R, U, F }, { L, U, B }, { L, U, F },
			{ R, D, B }, { R, D, F }, { L, D, F }, // DOWN
			{ R, D, B }, { L, D, F }, { L, D, B },
			{ L, D, F }, { L, U, F }, { L, U, B }, // LEFT
			{ L, D, F }, { L, U, B }, { L, D, B },
			{ R, D, B }, { R, U, B }, { R, U, F }, // RIGHT
			{ R, D, B }, { R, U, F }, { R, D, F },
			{ R, D, F }, { R, U, F }, { L, U, F }, // FRONT
			{ R, D, F }, { L, U, F }, { L, D, F },
			{ L, D, B }, { L, U, B }, { R, U, B }, // BACK
			{ L, D, B }, { R, U, B }, { R, D, B }
	};

	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		for (int j = 0; j < VECTOR_SIZE; j++)
		{
			m.vertices.push_back(vertices[i][j]);
		}
	}

	for (int j = 0; j < FACE_COUNT; j++)
	{
		pushBack(&m.normals, FACE_COUNT, (DIRECTIONS)j, VECTOR_SIZE);
	}

	m.color = this->color;
	m.renderType = GL_TRIANGLES;
	m.shader = this->shader;
	m.position = this->position;
	m.shininess = this->shininess;

	return m;
}

void pushBack(vector<GLfloat> *vector, int times, DIRECTIONS d, int arraySize)
{
	GLfloat *val = new GLfloat[arraySize];
	std::fill(val, val + arraySize, 0);

	switch (d)
	{
	case CUBE_DIRECTION_UP:
		val[1] = 1;
		break;
	case CUBE_DIRECTION_DOWN:
		val[1] = -1;
		break;
	case CUBE_DIRECTION_LEFT:
		val[0] = 1;
		break;
	case CUBE_DIRECTION_RIGHT:
		val[0] = -1;
		break;
	case CUBE_DIRECTION_FRONT:
		val[2] = -1;
		break;
	case CUBE_DIRECTION_BACK:
		val[2] = 1;
		break;
	}

	for (int i = 0; i < times; i++)
	{
		for (int j = 0; j < arraySize; j++)
		{
			vector->push_back(val[j]);
		}
	}
	
	delete val;
}