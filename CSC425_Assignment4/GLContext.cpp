#include "GLContext.h"
#include "LoadShaders.h"
#include "wglext.h"
#include <iostream>
#include <math.h>
#include <Windows.h>
#include <locale>
#include <codecvt>
#include <string>
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
			delete this->meshes;
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

	bool GLContext::initModels(const std::vector<InstancedMeshFactory> m)
	{
		if (this->success)
		{
			this->numVAOs = m.size();
			this->VAOs = new GLuint[this->numVAOs];
			this->numVBOs = 1;
			this->VBOs = new GLuint[this->numVBOs];
			this->meshes = new InstancedMeshFactory[this->numVAOs];

			glGenVertexArrays(this->numVAOs, this->VAOs);
			glGenBuffers(this->numVBOs, this->VBOs);

			// Gen texture

			GLuint tex;
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);

			// Load image

			float pixels[] = {
				.59, .42, .29,
				.47, .34, .23,
				.73, .52, .36,
				.59, .42, .29,
				.59, .42, .29,
				.47, .34, .23,
				.47, .34, .23,
				.73, .52, .36,
				.47, .34, .23,
				.59, .42, .29,
				.47, .34, .23,
				.47, .34, .23,
				.35, .24, .16,
				.35, .24, .16,
				.47, .34, .23,
				.59, .42, .29
			};
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 4, 0, GL_RGB, GL_FLOAT, pixels);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glGenerateMipmap(GL_TEXTURE_2D);

			GLfloat *texCoords = new GLfloat[72];

			for (int i = 0; i < 6; i++)
			{
				texCoords[i * 12] = 1;
				texCoords[i * 12 + 1] = 0;
				texCoords[i * 12 + 2] = 1;
				texCoords[i * 12 + 3] = 1;
				texCoords[i * 12 + 4] = 0;
				texCoords[i * 12 + 5] = 1;
				texCoords[i * 12 + 6] = 1;
				texCoords[i * 12 + 7] = 0;
				texCoords[i * 12 + 8] = 0;
				texCoords[i * 12 + 9] = 1;
				texCoords[i * 12 + 10] = 0;
				texCoords[i * 12 + 11] = 0;
			}

			// End load texture


			for (int i = 0; i < this->numVAOs; i++)
			{
				this->meshes[i] = m[i];

				glBindVertexArray(this->VAOs[i]);
				glBindBuffer(GL_ARRAY_BUFFER, this->VBOs[0]);

				std::size_t offset = 0;

				std::size_t vertexDataByteSize = this->meshes[i].vertexDataByteSize();
				std::size_t instanceDataByteSize = this->meshes[i].instanceDataByteSize();
				std::size_t vectorSize = this->meshes[i].vectorSize();
				std::size_t texDimens = this->meshes[i].texDimens();

				GLuint program = this->shaderPrograms[this->meshes[i].material];
				glUseProgram(program);

				glBufferData(GL_ARRAY_BUFFER, vertexDataByteSize + instanceDataByteSize, NULL, GL_STATIC_DRAW);

				glBufferSubData(GL_ARRAY_BUFFER, offset, this->meshes[i].verticesByteSize(), &this->meshes[i].vertices->front());
				GLuint vPosition = glGetAttribLocation(program, "vPosition");
				glEnableVertexAttribArray(vPosition);
				glVertexAttribPointer(vPosition, vectorSize, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(offset));

				offset += this->meshes[i].verticesByteSize();

				glBufferSubData(GL_ARRAY_BUFFER, offset, this->meshes[i].texCoordByteSize(), &this->meshes[i].texCoords->front());
				GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
				glEnableVertexAttribArray(vTexCoord);
				glVertexAttribPointer(vTexCoord, texDimens, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(offset));

				offset += this->meshes[i].texCoordByteSize();

				glBufferSubData(GL_ARRAY_BUFFER, offset, this->meshes[i].instanceOffsetByteSize(), &this->meshes[i].instanceOffset->front());
				GLuint iOffset = glGetAttribLocation(program, "iOffset");
				glEnableVertexAttribArray(iOffset);
				glVertexAttribPointer(iOffset, vectorSize, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(offset));

				offset += this->meshes[i].instanceOffsetByteSize();

				glBufferSubData(GL_ARRAY_BUFFER, offset, this->meshes[i].instanceRotationByteSize(), &this->meshes[i].instanceRotation->front());
				GLuint iRotation = glGetAttribLocation(program, "iRotation");
				glEnableVertexAttribArray(iRotation);
				glVertexAttribPointer(iRotation, vectorSize, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(offset));
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
			this->projectionTransform = glm::perspective<float>(45.0f, this->w / this->h, 0.01f, 100.0f);
			this->viewTransform = glm::mat4() * glm::lookAt(this->camera->getPosition(), this->camera->getGaze(), glm::vec3(0.0, 1.0, 0.0));

			glutMainLoop();
		}
	}

	void GLContext::render()
	{
		if (this->success)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.0, 0.0, 0.0, 1.0);

			int deltaTime = glutGet(GLUT_ELAPSED_TIME);

			glm::mat4 perspectiveTransform = this->projectionTransform * this->viewTransform;

			for (int i = 0; i < this->numVAOs; i++)
			{
				glBindVertexArray(this->VAOs[i]);

				GLuint program = this->shaderPrograms[this->meshes[i].material];
				glUseProgram(program);

				GLint perspectiveTransformLoc = glGetUniformLocation(program, "perspectiveTransform");

				glUniformMatrix4fv(perspectiveTransformLoc, 1, GL_FALSE, glm::value_ptr(perspectiveTransform));

				glDrawArraysInstanced(GL_TRIANGLES, 0, this->meshes[i].vertices->size(), this->meshes[i].numInstances());
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