#include "Cube.h"
#include <iostream>
using namespace std;

Cube::Cube(position3 origin, GLContext::vec3 scale, COLOR color, const char *shader)
{
	this->origin = origin;
	this->scale = scale;
	this->shader = shader;
	this->renderType = renderType;
	
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

	float scaleX = this->scale.x * 0.5f;
	float scaleY = this->scale.y * 0.5f;
	float scaleZ = this->scale.z * 0.5f;

	// courtesy of research publication "Optimizing Triangle Strips for Fast Rendering"
	// http://www.cs.umd.edu/gvil/papers/av_ts.pdf

	GLfloat cube[VERTEX_COUNT][VECTOR_SIZE] = {
			{ -scaleX, scaleY, scaleZ }, // T-L-B
			{ scaleX, scaleY, scaleZ }, // T-R-B
			{ -scaleX, scaleY, -scaleZ }, // T-L-F
			{ scaleX, scaleY, -scaleZ }, // T-R-F
			{ scaleX, -scaleY, -scaleZ }, // B-R-F
			{ scaleX, scaleY, scaleZ }, // T-R-B
			{ scaleX, -scaleY, scaleZ }, // B-R-B
			{ -scaleX, scaleY, scaleZ }, // T-L-B
			{ -scaleX, -scaleY, scaleZ }, // B-L-B
			{ -scaleX, scaleY, -scaleZ }, // T-L-F
			{ -scaleX, -scaleY, -scaleZ }, // B-L-F
			{ scaleX, -scaleY, -scaleZ }, // B-R-F
			{ -scaleX, -scaleY, scaleZ }, // B-L-B
			{ scaleX, -scaleY, scaleZ }  // B-R-B
	};
	
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		for (int j = 0; j < VECTOR_SIZE; j++)
		{
			m.vertices.push_back(cube[i][j]);
		}
	}

	m.color = this->color;
	m.renderType = GL_TRIANGLE_STRIP;
	m.shader = this->shader;
	m.position = this->origin;

	return m;
}