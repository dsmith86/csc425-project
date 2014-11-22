#pragma once
#include "InstancedMeshFactory.h"

#define CUBE_VERTEX_COUNT 36
#define CUBE_TEXTCOORD_DIMENS 2

class InstancedCubeFactory :
	public InstancedMeshFactory
{
public:
	void init(glm::vec3 scale, const char *texture, const char *material);
};

