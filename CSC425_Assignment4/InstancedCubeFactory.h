#pragma once
#include "InstancedMeshFactory.h"

#define CUBE_VERTEX_COUNT 36

class InstancedCubeFactory :
	public InstancedMeshFactory
{
public:
	void init(glm::vec3 scale, const char *texture, const char *material);
	std::size_t texCoordByteSize();
	std::size_t texDimens();
};

