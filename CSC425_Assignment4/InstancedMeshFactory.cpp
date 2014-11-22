#include "InstancedMeshFactory.h"


InstancedMeshFactory::InstancedMeshFactory()
{
	this->vertices = new std::vector<GLfloat>();
	this->texCoords = new std::vector<GLfloat>();
}


InstancedMeshFactory::~InstancedMeshFactory()
{
	delete vertices;
	delete texCoords;
}

void InstancedMeshFactory::init(const char *texture, const char *material)
{
	this->texture = texture;
	this->material = material;
}