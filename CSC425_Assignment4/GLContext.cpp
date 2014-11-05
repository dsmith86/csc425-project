#include "GLContext.h"
#include "LoadShaders.h"
#include <iostream>
#include <glm.hpp>
#include <mat4x4.hpp>
#include <vec3.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace GLContext {

	GLContext::GLContext()
	{
		this->success = false;
		this->shaderPrograms = std::unordered_map<const char*, GLuint>();
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


	bool GLContext::initContext(int argc, char** argv, displayFunc dFunc, reshapeFunc rFunc)
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGBA);
		glutInitWindowSize(512, 512);
		glutInitContextVersion(4, 3);
		glutInitContextProfile(GLUT_FORWARD_COMPATIBLE);
		glutCreateWindow(argv[0]);

		glutDisplayFunc(dFunc);
		glutReshapeFunc(rFunc);

		if (glewInit()) {
			std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
			this->success = false;
			return false;
		}

		vec3 cameraPosition;
		cameraPosition.x = 0;
		cameraPosition.y = 0;
		cameraPosition.z = -1;
		vec3 cameraGaze = { 0.0, 0.0, 0.0 };

		initCamera(cameraPosition, cameraGaze);

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
				int vertexCount = m[i].vertices.size();
				int normalCount = m[i].normals.size();

				glBindVertexArray(this->VAOs[i]);
				glBindBuffer(GL_ARRAY_BUFFER, this->VBOs[i]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (vertexCount), NULL, GL_STATIC_DRAW);

				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * vertexCount, &m[i].vertices.front());
				glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexCount, sizeof(GLfloat) * normalCount, &m[i].normals.front());

				GLuint program = this->shaderPrograms[m[i].shader];
				glUseProgram(program);

				GLuint vPosition = glGetAttribLocation(program, "vPosition");
				glEnableVertexAttribArray(vPosition);
				glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

				GLuint vNormal = glGetAttribLocation(program, "vNormal");
				glEnableVertexAttribArray(vNormal);
				glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(GLfloat) * vertexCount));



				this->models[i] = m[i];
			}

			return true;
		}
		return false;
	}

	void GLContext::initCamera(vec3 position, vec3 gaze)
	{
		if (!this->cameraInitialized)
		{
			delete camera;
		}
		this->camera = new Camera(glm::vec3(position.x, position.y, position.z), glm::vec3(gaze.x, gaze.y, gaze.z));
		this->cameraInitialized = true;

	}

	void GLContext::initLight(float x, float y, float z)
	{
		this->light = { x, y, z };
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
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 test = glm::lookAt(this->camera->getUp(), this->camera->getGaze(), this->camera->getUp());
			glm::mat4 test2 = glm::lookAt(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

			glm::mat4 proj = glm::perspective<float>(90.0f, this->w / this->h, 0.01f, 100.0f);
			glm::mat4 view = glm::mat4() * glm::lookAt(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

			for (int i = 0; i < this->numVAOs; i++)
			{
				model m = this->models[i];

				GLuint program = this->shaderPrograms[m.shader];
				glUseProgram(program);
				glBindVertexArray(this->VAOs[i]);

				GLint colLoc = glGetUniformLocation(program, "uColor");
				GLfloat color[4] = { m.color.x, m.color.y, m.color.z, 1.0 };
				glUniform4fv(colLoc, 1, color);

				GLint lightLoc = glGetUniformLocation(program, "lightSource");
				GLfloat light[4] = { this->light.x, this->light.y, this->light.z, 1.0 };
				glUniform3fv(lightLoc, 1, light);

				GLint projLoc = glGetUniformLocation(program, "projection");
				GLint viewLoc = glGetUniformLocation(program, "view");
				GLint modelLoc = glGetUniformLocation(program, "model");

				glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(m.position.x, m.position.y, m.position.z));
				glm::mat4 mod = translate;

				glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
				glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mod));

				glDrawArrays(this->models[i].renderType, 0, this->models[i].vertices.size() / VECTOR_SIZE);
			}

			glutSwapBuffers();
		}
	}

	void GLContext::reshape(int w, int h)
	{
		this->w = (float)w;
		this->h = (float)h;
	}

}