#include "GLContext.h"
#include "LoadShaders.h"
#include <iostream>
#include <math.h>
#include <glm.hpp>
#include <mat4x4.hpp>
#include <vec3.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace GLContext {

	bool validAttribLocation(GLuint location);

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
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
		glutInitWindowSize(768, 768);
		glutInitContextVersion(4, 3);
		glutInitContextProfile(GLUT_FORWARD_COMPATIBLE);
		glutCreateWindow(argv[0]);
		
		glEnable(GL_DEPTH_TEST);

		glutIdleFunc(dFunc);
		glutReshapeFunc(rFunc);

		if (glewInit()) {
			std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
			this->success = false;
			return false;
		}

		vec3 cameraPosition;
		cameraPosition.x = 0;
		cameraPosition.y = 0;
		cameraPosition.z = -3;
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
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (vertexCount + normalCount), NULL, GL_STATIC_DRAW);

				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * vertexCount, &m[i].vertices.front());
				glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexCount, sizeof(GLfloat) * normalCount, &m[i].normals.front());

				GLuint program = this->shaderPrograms[m[i].shader];
				glUseProgram(program);

				GLuint vPosition = glGetAttribLocation(program, "vPosition");
				glEnableVertexAttribArray(vPosition);
				glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

				GLuint vNormal = glGetAttribLocation(program, "vNormal");

				if (validAttribLocation(vNormal))
				{
					glEnableVertexAttribArray(vNormal);
					glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(GLfloat) * vertexCount));
				}
				

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
			glClearColor(0.0, 0.0, 0.0, 1.0);

			glm::mat4 proj = glm::perspective<float>(45.0f, this->w / this->h, 0.01f, 100.0f);
			glm::mat4 view = glm::mat4() * glm::lookAt(this->camera->getPosition(), this->camera->getGaze(), glm::vec3(0.0, 1.0, 0.0));

			for (int i = 0; i < this->numVAOs; i++)
			{
				model m = this->models[i];

				GLuint program = this->shaderPrograms[m.shader];
				glUseProgram(program);
				glBindVertexArray(this->VAOs[i]);

				GLint colLoc = glGetUniformLocation(program, "uColor");
				GLfloat color[4] = { m.color.x, m.color.y, m.color.z, 1.0 };
				glUniform4fv(colLoc, 1, color);

				GLint lightLoc = glGetUniformLocation(program, "LightPosition");
				GLfloat light[4] = { this->light.x, this->light.y, this->light.z, 1.0 };
				glUniform4fv(lightLoc, 1, light);

				GLint projLoc = glGetUniformLocation(program, "projection");
				GLint viewLoc = glGetUniformLocation(program, "view");
				GLint modelLoc = glGetUniformLocation(program, "model");

				int deltaTime = glutGet(GLUT_ELAPSED_TIME);
				float theta = fmod(m.rotationTheta * deltaTime, 360);

				glm::vec3 rotationTheta = glm::vec3(m.rotationAxis == DIRECTION::LEFT, m.rotationAxis == DIRECTION::UP, m.rotationAxis == DIRECTION::FRONT);
				glm::mat4 modelTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(m.position.x, m.position.y, m.position.z));
				glm::mat4 modelRotation = glm::rotate(modelTranslate, theta, rotationTheta);
				glm::mat4 mod = modelRotation;

				glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
				glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mod));

				float r = m.color.x;
				float g = m.color.y;
				float b = m.color.z;
				color4 light_ambient = glm::vec4(r, g, b, 1.0);
				color4 light_diffuse = color4(1.0, 1.0, 1.0, 1.0);
				color4 light_specular = color4(1.0, 1.0, 1.0, 1.0);

				color4 material_ambient = color4(1.0, 0.0, 1.0, 1.0);
				color4 material_diffuse = color4(1.0, 0.8, 0.0, 1.0);
				color4 material_specular = color4(1.0, 0.8, 0.0, 1.0);
				GLfloat material_shininess = this->shaderPrograms[m.shader];

				color4 ambient_product = light_ambient * material_ambient;
				color4 diffuse_product = light_diffuse * material_diffuse;
				color4 specular_product = light_specular * material_specular;

				GLfloat ambient[4] = { 0.1 * r, 0.1 * g, 0.1 * b, 1.0 };
				GLfloat diffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
				GLfloat specular[4] = { 1.0, 1.0, 1.0, 1.0 };

				glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient);
				glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse);
				glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular);

				glUniform1f(glGetUniformLocation(program, "Shininess"), m.shininess);

				glDrawArrays(this->models[i].renderType, 0, this->models[i].vertices.size() / VECTOR_SIZE);
			}

			glFlush();
			glutSwapBuffers();
		}
	}

	void GLContext::reshape(int w, int h)
	{
		this->w = (float)w;
		this->h = (float)h;
	}

	bool validAttribLocation(GLuint location)
	{
		int max_attribs;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_attribs);

		return location < (GLuint)max_attribs;
	}
}