#include <iostream>
#include "GLContext.h"
#include "Cube.h"
#include <math.h>
#include <algorithm>
#include "InstancedCubeFactory.h"

#define RADIANS_PER_DEGREE 0.017453293

using namespace std;

void display(void);
void reshape(int w, int h);

void arrange_in_circle(InstancedCubeFactory &meshes, int count, float radius);
void arrange_in_cube(InstancedCubeFactory &meshes, int x, int y, int z, int dimens);
void arrange_in_cube(InstancedCubeFactory &meshes, int count, int dimens);

GLContext::GLContext *glContext;

int main(int argc, char *argv[])
{
	glContext = new GLContext::GLContext();

	GLContext::material materials[] = {
			{ "texture", "texture.vert", "texture.frag" }
	};

	InstancedCubeFactory cubeFactory = InstancedCubeFactory();
	cubeFactory.init(glm::vec3(1.0, 1.0, 1.0), "dirt", "texture");

	//arrange_in_circle(models, 1000, 5);
	arrange_in_cube(cubeFactory, 2, 3);

	vector<InstancedMeshFactory> meshes = vector<InstancedMeshFactory>();
	meshes.push_back(cubeFactory);

	if (glContext->initContext(argc, argv, display, reshape) &&
		glContext->initShaders(materials, 1) &&
		glContext->initModels(meshes))
	{
		glContext->initCamera({ 0.0, 5.0, -15.0 }, { 0.0, 0.0, 0.0 });
		glContext->initLight(0, 0, -0.5);
		glContext->run();
	}

	delete glContext;

	return EXIT_SUCCESS;
}

void display(void)
{
	glContext->render();
}

void reshape(int w, int h)
{
	glContext->reshape(w, h);
}

void arrange_in_circle(InstancedCubeFactory &meshes, int count, float radius)
{
	if (count == 1)
	{
		meshes.addInstance(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
		return;
	}

	float theta = (360.0f / count);

	// the chord length of the circular segment with endpoints being
	// any two adjacent cubes
	float chordLength = 2 * radius * (float)sin(theta / 2 * RADIANS_PER_DEGREE);

	float cubeScale = min(1.0f, chordLength / 2);

	for (int i = 0; i < count; i++)
	{
		float direction = (i % 2 == 0) ? 0.1f : -0.1f;

		

		float x = radius * (float)cos(theta * i * RADIANS_PER_DEGREE);
		float y = radius * (float)sin(theta * i * RADIANS_PER_DEGREE);

		meshes.addInstance(glm::vec3(x, y, 0.0), glm::vec3(0, 0, 0));
	}
}

void arrange_in_cube(InstancedCubeFactory &meshes, int count, int dimens)
{
	arrange_in_cube(meshes, count, count, count, dimens);
}

void arrange_in_cube(InstancedCubeFactory &meshes, int x, int y, int z, int dimens)
{
	float deltaX = (float)dimens / x * 2;
	float deltaY = (float)dimens / y * 2;
	float deltaZ = (float)dimens / z * 2;

	float cubeScale = min(1.0f, deltaX / 1.5f);

	cout << "scale: " << cubeScale << endl;

	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			for (int k = 0; k < z; k++)
			{
				float xCoord = (i <= x / 2.0) ? -dimens + deltaX * i + deltaX / 2 : dimens - deltaX * (x - i) + deltaX / 2;
				float yCoord = (j <= y / 2.0) ? -dimens + deltaY * j + deltaY / 2 : dimens - deltaY * (y - j) + deltaY / 2;
				float zCoord = (k <= z / 2.0) ? -dimens + deltaZ * k + deltaZ / 2 : dimens - deltaZ * (z - k) + deltaZ / 2;

				meshes.addInstance(glm::vec3(xCoord, yCoord, zCoord), glm::vec3(0, 0, 0));
			}
		}
	}
}