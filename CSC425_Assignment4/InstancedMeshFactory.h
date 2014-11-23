#pragma once

#include <vector>
#include <glm.hpp>
#include "vgl.h"

#define MESH_VECTOR_SIZE 3
#define TEXCOORD_DIMENS 2

class InstancedMeshFactory
{
public:
	InstancedMeshFactory();
	~InstancedMeshFactory();
	// Generator functions
	void init(const char *texture, const char *material);
	int addInstance(glm::vec3 offset, glm::vec3 rotation);
	// Properties
	std::vector<GLfloat> *vertices;
	std::vector<GLfloat> *texCoords;
	std::vector<GLfloat> *instanceOffset;
	std::vector<GLfloat> *instanceRotation;
	const char *material;
	// Accessor Functions
	std::size_t vectorSize();
	std::size_t texDimens();
	std::size_t vertexDataByteSize();
	std::size_t instanceDataByteSize();
	std::size_t verticesByteSize();
	std::size_t texCoordByteSize();
	std::size_t instanceOffsetByteSize();
	std::size_t instanceRotationByteSize();
	std::size_t numInstances();
protected:
	const char *texture;
	GLenum renderType;
private:
	std::size_t instanceCount;
};

