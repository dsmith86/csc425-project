#include "Cube.h"
#include <iostream>
using namespace std;

Cube::Cube(position3 origin, vec3 scale, COLOR color)
{
	this->origin = origin;
	this->scale = scale;
	
	switch (color)
	{
	case COLOR::RED:
		this->color = { 1.0, 0.0, 0.0 };
		break;
	case COLOR::GREEN:
		this->color = { 0.0, 1.0, 0.0 };
		break;
	case COLOR::BLUE:
		this->color = { 0.0, 0.0, 0.0 };
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
	this->mModel = new model;

	float scaleX = this->scale.x * 0.5;
	float scaleY = this->scale.y * 0.5;
	float scaleZ = this->scale.z * 0.5;

	float originX = this->origin.x;
	float originY = this->origin.y;
	float originZ = this->origin.z;

	cout << "scaleX: " << scaleX << endl;
	cout << "scaleY: " << scaleY << endl;
	cout << "scaleZ: " << scaleZ << endl;

	cout << "originX: " << originX << endl;
	cout << "originY: " << originY << endl;
	cout << "originZ: " << originZ << endl;

	GLfloat cube[36][VECTOR_SIZE] = {
			{ originX - scaleX, originY - scaleY, originZ - scaleZ }, // LEFT
			{ originX - scaleX, originY + scaleY, originZ - scaleZ },
			{ originX - scaleX, originY + scaleY, originZ + scaleZ },
			{ originX - scaleX, originY - scaleY, originZ - scaleZ },
			{ originX - scaleX, originY + scaleY, originZ + scaleZ },
			{ originX - scaleX, originY - scaleY, originZ + scaleZ },
			{ originX + scaleX, originY - scaleY, originZ + scaleZ }, // RIGHT
			{ originX + scaleX, originY + scaleY, originZ + scaleZ },
			{ originX + scaleX, originY + scaleY, originZ - scaleZ },
			{ originX + scaleX, originY - scaleY, originZ + scaleZ },
			{ originX + scaleX, originY + scaleY, originZ - scaleZ },
			{ originX + scaleX, originY - scaleY, originZ - scaleZ },
			{ originX + scaleX, originY - scaleY, originZ - scaleZ }, // FRONT
			{ originX + scaleX, originY + scaleY, originZ - scaleZ },
			{ originX - scaleX, originY + scaleY, originZ - scaleZ },
			{ originX + scaleX, originY - scaleY, originZ - scaleZ },
			{ originX - scaleX, originY + scaleY, originZ - scaleZ },
			{ originX - scaleX, originY - scaleY, originZ - scaleZ },
			{ originX - scaleX, originY - scaleY, originZ + scaleZ }, // BACK
			{ originX - scaleX, originY + scaleY, originZ + scaleZ },
			{ originX + scaleX, originY + scaleY, originZ + scaleZ },
			{ originX - scaleX, originY - scaleY, originZ + scaleZ },
			{ originX + scaleX, originY + scaleY, originZ + scaleZ },
			{ originX + scaleX, originY - scaleY, originZ + scaleZ },
			{ originX + scaleX, originY + scaleY, originZ - scaleZ }, // TOP
			{ originX + scaleX, originY + scaleY, originZ + scaleZ },
			{ originX - scaleX, originY + scaleY, originZ + scaleZ },
			{ originX + scaleX, originY + scaleY, originZ - scaleZ },
			{ originX - scaleX, originY + scaleY, originZ + scaleZ },
			{ originX - scaleX, originY + scaleY, originZ - scaleZ },
			{ originX + scaleX, originY - scaleY, originZ + scaleZ }, // BOTTOM
			{ originX + scaleX, originY - scaleY, originZ - scaleZ },
			{ originX - scaleX, originY - scaleY, originZ - scaleZ },
			{ originX + scaleX, originY - scaleY, originZ + scaleZ },
			{ originX - scaleX, originY - scaleY, originZ - scaleZ },
			{ originX - scaleX, originY - scaleY, originZ + scaleZ }
	};

	this->mModel->vertices = *cube;
	this->mModel->numVertices = 36;
	this->mModel->color = this->color;

	return *(this->mModel);
}