#include "GLContext.h"
#include "LoadShaders.h"
#include <iostream>


GLContext::GLContext()
{
	this->success = false;
	this->shaderPrograms = unordered_map<const char*, GLuint>();
}


GLContext::~GLContext()
{
}


bool GLContext::initContext(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(1280, 720);
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_FORWARD_COMPATIBLE);
	glutCreateWindow(argv[0]);

	if (glewInit()) {
		cerr << "Unable to initialize GLEW ... exiting" << endl;
		this->success = false;
		return false;
	}

	this->success = true;
	return true;
}

bool GLContext::initShaders(const shaderType s[], int n)
{
	if (this->success)
	{
		for (int i = 0; i < n; i++)
		{
			ShaderInfo shaders[] = {
					{ GL_VERTEX_SHADER, s[i].vertexShader },
					{ GL_FRAGMENT_SHADER, s[i].fragmentShader },
					{ GL_NONE, NULL }
			};

			GLuint program = LoadShaders(shaders);

			std::pair<const char*, GLuint> shaderProgram(s[i].shaderName, program);

			this->shaderPrograms.insert(shaderProgram);
		}

		return true;
	}
	return false;
}