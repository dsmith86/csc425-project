#include <iostream>
#include "GLContext.h"
#include "Cube.h"

using namespace std;

void display(void);
void reshape(int w, int h);

GLContext::GLContext *glContext;

int main(int argc, char *argv[])
{
	glContext = new GLContext::GLContext();

	GLContext::material materials[] = {
			{ "texture", "texture.vert", "basic.frag" }
	};

	vector<GLContext::model> models = vector<GLContext::model>();

	models.push_back(Cube({ 0.0, 0.0, 0.0 }, { 1.0, 1.0, 1.0 }, COLOR::RED, "texture", NULL).modelData()); // texture
	
	if (glContext->initContext(argc, argv, display, reshape) &&
		glContext->initShaders(materials, 1) &&
		glContext->initModels(&models.front(), models.size()));
	{
		glContext->initCamera({ 2.0, 2.0, -5.0 }, { 0.0, 0.0, 0.0 });
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