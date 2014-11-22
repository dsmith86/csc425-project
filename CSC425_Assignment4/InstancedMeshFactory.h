#pragma once

#include <vector>
#include <glm.hpp>
#include "vgl.h"

#define MESH_VECTOR_SIZE 3

class InstancedMeshFactory
{
public:
	InstancedMeshFactory();
	~InstancedMeshFactory();
	// Generator functions
	void init(const char *texture, const char *material);
	unsigned int addInstance(glm::vec3 offset, glm::vec3 rotation);
	// Accessor functions

protected:
	std::vector<GLfloat> *vertices;
	std::vector<GLfloat> *texCoords;
	const char *texture;
	const char *material;
	GLenum renderType;
};

