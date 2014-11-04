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

		float originX = 0, originY = 0, originZ = 0;
		float scaleX = 0.5, scaleY = 0.5, scaleZ = 0.5;

		GLfloat verts[36][VECTOR_SIZE] = {
				{ originX - scaleX, originY - scaleY, originZ - scaleZ }, // LEFT
				{ originX - scaleX, originY + scaleY, originZ - scaleZ },
				{ originX - scaleX, originY + scaleY, originZ + scaleZ },
				{ originX - scaleX, originY - scaleY, originZ - scaleZ },
				{ originX - scaleX, originY + scaleY, originZ + scaleZ },
				{ originX - scaleX, originY - scaleY, originZ + scaleZ },
				{ originX + scaleX, originY - scaleY, originZ + scaleZ }, // RIGHT
				{ originX + scaleX, originY + scaleY, originZ + scaleZ },
				{ originX + scaleX, originY + scaleY, originZ - scaleZ },
				{ originX + scaleX, originY - scaleY, originZ + scaleZ },
				{ originX + scaleX, originY + scaleY, originZ - scaleZ },
				{ originX + scaleX, originY - scaleY, originZ - scaleZ },
				{ originX + scaleX, originY - scaleY, originZ - scaleZ }, // FRONT
				{ originX + scaleX, originY + scaleY, originZ - scaleZ },
				{ originX - scaleX, originY + scaleY, originZ - scaleZ },
				{ originX + scaleX, originY - scaleY, originZ - scaleZ },
				{ originX - scaleX, originY + scaleY, originZ - scaleZ },
				{ originX - scaleX, originY - scaleY, originZ - scaleZ },
				{ originX - scaleX, originY - scaleY, originZ + scaleZ }, // BACK
				{ originX - scaleX, originY + scaleY, originZ + scaleZ },
				{ originX + scaleX, originY + scaleY, originZ + scaleZ },
				{ originX - scaleX, originY - scaleY, originZ + scaleZ },
				{ originX + scaleX, originY + scaleY, originZ + scaleZ },
				{ originX + scaleX, originY - scaleY, originZ + scaleZ },
				{ originX + scaleX, originY + scaleY, originZ - scaleZ }, // TOP
				{ originX + scaleX, originY + scaleY, originZ + scaleZ },
				{ originX - scaleX, originY + scaleY, originZ + scaleZ },
				{ originX + scaleX, originY + scaleY, originZ - scaleZ },
				{ originX - scaleX, originY + scaleY, originZ + scaleZ },
				{ originX - scaleX, originY + scaleY, originZ - scaleZ },
				{ originX + scaleX, originY - scaleY, originZ + scaleZ }, // BOTTOM
				{ originX + scaleX, originY - scaleY, originZ - scaleZ },
				{ originX - scaleX, originY - scaleY, originZ - scaleZ },
				{ originX + scaleX, originY - scaleY, originZ + scaleZ },
				{ originX - scaleX, originY - scaleY, originZ - scaleZ },
				{ originX - scaleX, originY - scaleY, originZ + scaleZ }
		};

		// I can't get the pointer syntax right, so for now I'm resetting the verts here
		cubeModel.vertices = *verts;

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