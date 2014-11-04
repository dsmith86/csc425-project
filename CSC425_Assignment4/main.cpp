#include <iostream>
#include "GLContext.h"
#include "Cube.h"
using namespace std;

void display(void);

GLContext *glContext;

int main(int argc, char *argv[])
{
	glContext = new GLContext();
	
	if (glContext->initContext(argc, argv, display))
	{
		material materials[] = {
				{ "basic", "basic.vert", "basic.frag" }
		};

		glContext->initShaders(materials, 1);

		Cube cube = Cube({ 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, COLOR::RED);

		model cubeModel = cube.modelData();
		cubeModel.shader = "basic";

		model models[1];
		models[0] = cubeModel;

		glContext->initModels(models, 1);

		glContext->run();
	}

	delete glContext;

	return EXIT_SUCCESS;
}

void display(void)
{
	glContext->render();
}