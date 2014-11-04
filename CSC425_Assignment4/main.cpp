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

		vector<model> models = vector<model>();

		models.push_back(Cube({ -0.5, 0.0, 0.0 }, { 0.1, 1.0, 1.0 }, COLOR::RED, "basic").modelData());
		models.push_back(Cube({ 0.5, 0.0, 0.0 }, { 0.1, 1.0, 1.0 }, COLOR::GREEN, "basic").modelData());
		models.push_back(Cube({ 0.0, 0.5, 0.0 }, { 0.1, 1.0, 1.0 }, COLOR::BLUE, "basic").modelData());

		glContext->initModels(&models.front(), models.size());

		glContext->run();
	}

	delete glContext;

	return EXIT_SUCCESS;
}

void display(void)
{
	glContext->render();
}