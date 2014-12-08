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
void mouseStateChanged(int button, int state, int x, int y);
void keyPressed(unsigned char key, int x, int y);
void keyReleased(unsigned char key, int x, int y);
void keySpecial(int key, int x, int y);
void keySpecialUp(int key, int x, int y);

void arrange_in_circle(modelBag &models, int count, float radius);
void place_cubes(modelBag &models, int x, int y, int z, int dimens, glm::vec3 origin, const char *material);
void place_cubes(modelBag &models, int count, int dimens, glm::vec3 origin, const char *material);

GLContext::GLContext *glContext;

int main(int argc, char *argv[])
{
	glContext = new GLContext::GLContext();

	GLContext::material materials[] = {
			{ "wood", "texture.vert", "texture.frag", "wood.png", NULL, 1 },
			{ "rubiks", "texture.vert", "texture.frag", "rubiks.png", NULL, 1 },
			{ "rust", "texture.vert", "texture.frag", "rust.png", NULL, 1 },
			{ "fabric", "texture.vert", "texture.frag", "fabric.png", NULL, 1 },
			{ "nicolascage", "texture.vert", "texture.frag", "nicolascage.png", NULL, 1 },
			{ "ground", "texture.vert", "texture.frag", "ground.png", NULL, 100 },
			{ "sky", "texture.vert", "texture.frag", "sky.png", NULL, 1 },
			{ "ceiling", "texture.vert", "texture.frag", "ceiling.png", NULL, 1 },
			{ "southernmiss", "texture.vert", "texture.frag", "southernmiss.png", NULL, 1 }
	};

	modelBag models = modelBag();

	//arrange_in_circle(models, 1000, 5);
	place_cubes(models, 1, 1, glm::vec3(0, 0, 0), "wood");
	place_cubes(models, 1, 1, glm::vec3(5, 0, 0), "rubiks");
	place_cubes(models, 1, 10, glm::vec3(-20, 0, 0), "rust");
	place_cubes(models, 1, 5, glm::vec3(20, 0, 0), "fabric");
	place_cubes(models, 4, 3, glm::vec3(2, 0, 10), "nicolascage");
	place_cubes(models, 1, 3, glm::vec3(0, 5, -10), "southernmiss");
	
	models.push_back(Cube({ 0, -0.75, 0 }, { 200, 0.01, 200 }, "ground", 0, 1).setRotation(GLContext::DIRECTION::UP, 0.0)->modelData());
	models.push_back(Cube({ 0, 0, 100 }, { 200, 100, 1 }, "sky", 0, 1).setRotation(GLContext::DIRECTION::UP, 0.0)->modelData());
	models.push_back(Cube({ 100, 0, 0 }, { 1, 100, 200 }, "sky", 0, 1).setRotation(GLContext::DIRECTION::UP, 0.0)->modelData());
	models.push_back(Cube({ 0, 0, -100 }, { 200, 100, 1 }, "sky", 0, 1).setRotation(GLContext::DIRECTION::UP, 0.0)->modelData());
	models.push_back(Cube({ -100, 0, 0 }, { 1, 100, 200 }, "sky", 0, 1).setRotation(GLContext::DIRECTION::UP, 0.0)->modelData());
	models.push_back(Cube({ 0, 50, 0 }, { 200, 1, 200 }, "ceiling", 0, 1).setRotation(GLContext::DIRECTION::UP, 0.0)->modelData());



	if (glContext->initContext(argc, argv, display, reshape, mouseMoved, mouseStateChanged, keyPressed, keyReleased, keySpecial, keySpecialUp) &&
		glContext->initShaders(materials, 9) &&
		glContext->initModels(&models.front(), models.size()));
	{
		glContext->initCamera(glm::vec3(0.0, HEIGHT_FROM_GROUND, -15.0), glm::vec3(0.0, HEIGHT_FROM_GROUND, 0.0));
		glContext->initLight(glm::vec3(2, 2.0, -5.0));
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
	glContext->rotateCamera(x, y, 0.3);
}

void mouseStateChanged(int button, int state, int x, int y)
{
	glContext->mouseStateChanged(button, state);
}

void keyPressed(unsigned char key, int x, int y)
{
	glContext->keyPressed(key);
}

void keyReleased(unsigned char key, int x, int y)
{
	glContext->keyReleased(key);
}

void keySpecial(int key, int x, int y)
{
	glContext->keySpecial(key);
}

void keySpecialUp(int key, int x, int y)
{
	glContext->keySpecialUp(key);
}

void arrange_in_circle(modelBag &models, int count, float radius)
{
	if (count == 1)
	{
		models.push_back(Cube({ 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, "wood", 0, 1).setRotation(GLContext::DIRECTION::UP, 0.0)->modelData());
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

		models.push_back(Cube({ x, y, 0.0 }, { cubeScale, cubeScale, cubeScale }, "wood", 0, 1).setRotation(GLContext::DIRECTION::UP, direction)->modelData());
	}
}

void place_cubes(modelBag &models, int count, int dimens, glm::vec3 origin, const char *material)
{
	place_cubes(models, count, count, count, dimens, origin, material);
}

void place_cubes(modelBag &models, int x, int y, int z, int dimens, glm::vec3 origin, const char *material)
{
	float deltaX = (float)dimens / x * 2;
	float deltaY = (float)dimens / y * 2;
	float deltaZ = (float)dimens / z * 2;

	float cubeScale = deltaX / 1.5f;

	float yOffset = (y > 1) ? cubeScale : 0;

	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			for (int k = 0; k < z; k++)
			{
				float xCoord = (i <= x / 2.0) ? -dimens + deltaX * i + deltaX / 2 : dimens - deltaX * (x - i) + deltaX / 2;
				float yCoord = (j <= y / 2.0) ? -dimens + deltaY * j + deltaY / 2 : dimens - deltaY * (y - j) + deltaY / 2;
				float zCoord = (k <= z / 2.0) ? -dimens + deltaZ * k + deltaZ / 2 : dimens - deltaZ * (z - k) + deltaZ / 2;

				// I'm offsetting the yCoord to account for the ground plane
				models.push_back(Cube({ xCoord + origin.x, yCoord + dimens / 2 + origin.y + yOffset, zCoord + origin.z }, { cubeScale, cubeScale, cubeScale }, material, 0, 1).setRotation(GLContext::DIRECTION::UP, 0.0)->modelData());
			}
		}
	}
}