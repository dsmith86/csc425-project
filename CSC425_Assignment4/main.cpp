#include <iostream>
#include "GLContext.h"
#include "Cube.h"
#include <math.h>
#include <algorithm>
#include "InstancedCubeFactory.h"

#define RADIANS_PER_DEGREE 0.017453293

using namespace std;

typedef vector<GLContext::model> modelBag;

void display(void);
void reshape(int w, int h);
void mouseMoved(int x, int y);
void keyPressed(unsigned char key, int x, int y);

void arrange_in_circle(modelBag &models, int count, float radius);
void arrange_in_cube(modelBag &models, int x, int y, int z, int dimens);
void arrange_in_cube(modelBag &models, int count, int dimens);

GLContext::GLContext *glContext;

int main(int argc, char *argv[])
{
	glContext = new GLContext::GLContext();

	GLContext::material materials[] = {
			{ "texture", "texture.vert", "texture.frag" }
	};

	InstancedCubeFactory cubeFactory = InstancedCubeFactory();
	cubeFactory.init(glm::vec3(1.0, 1.0, 1.0), "dirt", "texture");

	modelBag models = modelBag();

	//arrange_in_circle(models, 1000, 5);
	arrange_in_cube(models, 5, 4);

	if (glContext->initContext(argc, argv, display, reshape, mouseMoved, keyPressed) &&
		glContext->initShaders(materials, 1) &&
		glContext->initModels(&models.front(), models.size()));
	{
		glContext->initCamera(glm::vec3(0.0, 0.0, -15.0), glm::vec3(0.0, 0.0, 0.0));
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

void mouseMoved(int x, int y)
{
	glContext->rotateCamera(x, y, 0.1);
}

void keyPressed(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		glContext->moveCamera(Camera::DIRECTION::FORWARD);
		break;
	case 'a':
		glContext->moveCamera(Camera::DIRECTION::LEFT);
		break;
	case 's':
		glContext->moveCamera(Camera::DIRECTION::BACK);
		break;
	case 'd':
		glContext->moveCamera(Camera::DIRECTION::RIGHT);
		break;
	case 'q':
		glContext->quit();
		break;
	}
}

void arrange_in_circle(modelBag &models, int count, float radius)
{
	if (count == 1)
	{
		models.push_back(Cube({ 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, "dirt", "texture").setRotation(GLContext::DIRECTION::UP, 0.1)->modelData());
		return;
	}

	float theta = (360.0 / count);

	// the chord length of the circular segment with endpoints being
	// any two adjacent cubes
	float chordLength = 2 * radius * sin(theta / 2 * RADIANS_PER_DEGREE);

	float cubeScale = min(1.0f, chordLength / 2);

	for (int i = 0; i < count; i++)
	{
		float direction = (i % 2 == 0) ? 0.1 : -0.1;

		

		float x = radius * cos(theta * i * RADIANS_PER_DEGREE);
		float y = radius * sin(theta * i * RADIANS_PER_DEGREE);

		models.push_back(Cube({ x, y, 0.0 }, { cubeScale, cubeScale, cubeScale }, "dirt", "texture").setRotation(GLContext::DIRECTION::UP, direction)->modelData());
	}
}

void arrange_in_cube(modelBag &models, int count, int dimens)
{
	arrange_in_cube(models, count, count, count, dimens);
}

void arrange_in_cube(modelBag &models, int x, int y, int z, int dimens)
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

				models.push_back(Cube({ xCoord, yCoord, zCoord }, { cubeScale, cubeScale, cubeScale }, "dirt", "texture").setRotation(GLContext::DIRECTION::UP, 0.0)->modelData());
			}
		}
	}
}