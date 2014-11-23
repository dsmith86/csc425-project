#include "InstancedCubeFactory.h"

void InstancedCubeFactory::init(glm::vec3 scale, const char *texture, const char *material)
{
	InstancedMeshFactory::init(texture, material);
	GLfloat *texCoords;

	float L = scale.x * 0.5f;
	float U = scale.y * 0.5f;
	float B = scale.z * 0.5f;
	float R = -L;
	float D = -U;
	float F = -B;

	// courtesy of research publication "Optimizing Triangle Strips for Fast Rendering"
	// http://www.cs.umd.edu/gvil/papers/av_ts.pdf

	GLfloat vertices[CUBE_VERTEX_COUNT * MESH_VECTOR_SIZE] = {
		R, U, F, R, U, B, L, U, B, // UP
		R, U, F, L, U, B, L, U, F,
		R, D, B, R, D, F, L, D, F, // DOWN
		R, D, B, L, D, F, L, D, B,
		L, D, F, L, U, F, L, U, B, // LEFT
		L, D, F, L, U, B, L, D, B,
		R, D, B, R, U, B, R, U, F, // RIGHT
		R, D, B, R, U, F, R, D, F,
		R, D, F, R, U, F, L, U, F, // FRONT
		R, D, F, L, U, F, L, D, F,
		L, D, B, L, U, B, R, U, B, // BACK
		L, D, B, R, U, B, R, D, B
	};

	texCoords = new GLfloat[CUBE_VERTEX_COUNT * TEXCOORD_DIMENS];

	for (int i = 0; i < 6; i++)
	{
		texCoords[i * 12] = 1;
		texCoords[i * 12 + 1] = 0;
		texCoords[i * 12 + 2] = 1;
		texCoords[i * 12 + 3] = 1;
		texCoords[i * 12 + 4] = 0;
		texCoords[i * 12 + 5] = 1;
		texCoords[i * 12 + 6] = 1;
		texCoords[i * 12 + 7] = 0;
		texCoords[i * 12 + 8] = 0;
		texCoords[i * 12 + 9] = 1;
		texCoords[i * 12 + 10] = 0;
		texCoords[i * 12 + 11] = 0;
	}

	this->vertices->assign(vertices, vertices + CUBE_VERTEX_COUNT * MESH_VECTOR_SIZE);
	this->texCoords->assign(texCoords, texCoords + CUBE_VERTEX_COUNT * TEXCOORD_DIMENS);

	this->renderType = GL_TRIANGLES;

	delete texCoords;
}



