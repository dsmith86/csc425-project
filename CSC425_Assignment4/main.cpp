#include <iostream>
#include "GLContext.h"
#include "Cube.h"
#include <math.h>
#include <algorithm>

#define RADIANS_PER_DEGREE 0.017453293

using namespace std;

typedef vector<GLContext::model> modelBag;

void display(void);
void reshape(int w, int h);

void arrange_in_circle(modelBag &models, int count, float radius);

GLContext::GLContext *glContext;

int main(int argc, char *argv[])
{
	glContext = new GLContext::GLContext();

	GLContext::material materials[] = {
			{ "texture", "texture.vert", "texture.frag" }
	};

	modelBag models = modelBag();

	arrange_in_circle(models, 1000, 5);

	if (glContext->initContext(argc, argv, display, reshape) &&
		glContext->initShaders(materials, 1) &&
		glContext->initModels(&models.front(), models.size()));
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