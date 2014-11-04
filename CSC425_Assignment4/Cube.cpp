#include "Cube.h"
#include <iostream>
using namespace std;

Cube::Cube(position3 origin, vec3 scale, COLOR color, const char *shader)
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

model Cube::modelData()
{
	model m;

	float scaleX = this->scale.x * 0.5f;
	float scaleY = this->scale.y * 0.5f;
	float scaleZ = this->scale.z * 0.5f;

	float originX = this->origin.x;
	float originY = this->origin.y;
	float originZ = this->origin.z;

	cout << "scaleX: " << scaleX << endl;
	cout << "scaleY: " << scaleY << endl;
	cout << "scaleZ: " << scaleZ << endl;

	cout << "originX: " << originX << endl;
	cout << "originY: " << originY << endl;
	cout << "originZ: " << originZ << endl;

	// courtesy of research publication "Optimizing Triangle Strips for Fast Rendering"
	// http://www.cs.umd.edu/gvil/papers/av_ts.pdf

	GLfloat cube[VERTEX_COUNT][VECTOR_SIZE] = {
			{ originX - scaleX, originY + scaleY, originZ + scaleZ }, // T-L-B
			{ originX + scaleX, originY + scaleY, originZ + scaleZ }, // T-R-B
			{ originX - scaleX, originY + scaleY, originZ - scaleZ }, // T-L-F
			{ originX + scaleX, originY + scaleY, originZ - scaleZ }, // T-R-F
			{ originX + scaleX, originY - scaleY, originZ - scaleZ }, // B-R-F
			{ originX + scaleX, originY + scaleY, originZ + scaleZ }, // T-R-B
			{ originX + scaleX, originY - scaleY, originZ + scaleZ }, // B-R-B
			{ originX - scaleX, originY + scaleY, originZ + scaleZ }, // T-L-B
			{ originX - scaleX, originY - scaleY, originZ + scaleZ }, // B-L-B
			{ originX - scaleX, originY + scaleY, originZ - scaleZ }, // T-L-F
			{ originX - scaleX, originY - scaleY, originZ - scaleZ }, // B-L-F
			{ originX + scaleX, originY - scaleY, originZ - scaleZ }, // B-R-F
			{ originX - scaleX, originY - scaleY, originZ + scaleZ }, // B-L-B
			{ originX + scaleX, originY - scaleY, originZ + scaleZ }  // B-R-B
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

	return m;
}