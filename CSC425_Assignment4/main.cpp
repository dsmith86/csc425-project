#include <iostream>
#include "GLContext.h"
using namespace std;

void display(void);

GLContext *glContext;

int main(int argc, char *argv[])
{
	glContext = new GLContext();
	
	if (glContext->initContext(argc, argv, display))
	{
		material materials[] = {
				{ "basic", "basic.vert", "basic.frag" },
				{ "other", "basic.vert", "basic.frag" }
		};

		glContext->initShaders(materials, 2);

		GLfloat triangleA[3][3] = {
				{ -0.9, -0.9, 0.0},
				{ -0.1, -0.9, 0.0 },
				{ -0.5, -0.1, 0.0}
		};

		GLfloat triangleB[3][3] = {
				{ 0.1, 0.1, 0.0 },
				{ 0.9, 0.1, 0.0 },
				{ 0.5, 0.9, 0.0}
		};

		model models[] = {
				{ *triangleA, 3, "basic" },
				{ *triangleB, 3, "other" }
		};

		glContext->initModels(models, 2);

		glContext->run();
	}

	delete glContext;

	return EXIT_SUCCESS;
}

void display(void)
{
	glContext->render();
}