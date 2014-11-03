#include <iostream>
#include "GLContext.h"
using namespace std;

int main(int argc, char *argv[])
{
	GLContext glContext = GLContext();
	
	if (glContext.initContext(argc, argv))
	{
		shaderType shaders[] = {
				{ "basic", "basic.vert", "basic.frag" }
		};

		glContext.initShaders(shaders, 1);
	}

	return EXIT_SUCCESS;
}