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
	

	if (this->numVAOs > 0)
	{
		glDeleteVertexArrays(this->numVAOs, this->VAOs);
		glDeleteBuffers(this->numVBOs, this->VBOs);
		delete this->VAOs;
		delete this->VBOs;
		delete this->models;
	}
}


bool GLContext::initContext(int argc, char** argv, displayFunc dFunc)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_FORWARD_COMPATIBLE);
	glutCreateWindow(argv[0]);

	glutDisplayFunc(dFunc);

	if (glewInit()) {
		cerr << "Unable to initialize GLEW ... exiting" << endl;
		this->success = false;
		return false;
	}

	this->success = true;
	return true;
}

bool GLContext::initShaders(const material s[], int n)
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

			std::pair<const char*, GLuint> shaderProgram(s[i].materialName, program);

			this->shaderPrograms.insert(shaderProgram);
		}

		return true;
	}
	return false;
}

bool GLContext::initModels(const model m[], int n)
{
	if (this->success)
	{
		

		this->numVAOs = n;
		this->numVBOs = n;
		this->VAOs = new GLuint[n];
		this->VBOs = new GLuint[n];
		this->models = new model[n];

		glGenVertexArrays(this->numVAOs, this->VAOs);

		glGenBuffers(n, this->VBOs);

		for (int i = 0; i < n; i++)
		{
			glBindVertexArray(this->VAOs[i]);
			glBindBuffer(GL_ARRAY_BUFFER, this->VBOs[i]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m[i].vertices.size(), &m[i].vertices.front(), GL_STATIC_DRAW);

			GLuint program = this->shaderPrograms[m[i].shader];
			glUseProgram(program);

			GLuint posLoc = glGetAttribLocation(program, "vPosition");
			glEnableVertexAttribArray(posLoc);
			glVertexAttribPointer(posLoc, VECTOR_SIZE, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

			

			this->models[i] = m[i];
		}

		return true;
	}
	return false;
}

void GLContext::run()
{
	if (this->success)
	{
		glutMainLoop();
	}
}

void GLContext::render()
{
	if (this->success)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		

		for (int i = 0; i < this->numVAOs; i++)
		{
			model m = this->models[i];

			GLuint program = this->shaderPrograms[m.shader];
			glUseProgram(program);
			glBindVertexArray(this->VAOs[i]);

			GLint mLoc = glGetUniformLocation(program, "vModel");
			GLfloat model[4][4] = {
				1.0, 0.0, 0.0, m.position.x,
				0.0, 1.0, 0.0, m.position.y,
				0.0, 0.0, 0.0, m.position.z,
				0.0, 0.0, 0.0, 1.0
			};
			glUniformMatrix4fv(mLoc, 1, GL_FALSE, *model);

			GLint colLoc = glGetUniformLocation(program, "uColor");
			GLfloat color[4] = { m.color.x, m.color.y, m.color.z, 1.0 };
			glUniform4fv(colLoc, 1, color);

			glDrawArrays(this->models[i].renderType, 0, this->models[i].vertices.size() / VECTOR_SIZE);
		}

		glFlush();
	}
}