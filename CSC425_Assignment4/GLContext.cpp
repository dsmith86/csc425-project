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
#include "stb_image.h"

namespace GLContext {

	void loadImageFromFile(GLuint &loc, std::string filename);
	bool validAttribLocation(GLuint location);

	GLContext::GLContext()
	{
		this->success = false;
		this->shaderPrograms = std::unordered_map<const char*, GLuint>();
		this->textures = std::unordered_map<const char*, GLuint>();
		this->tiling = std::unordered_map<const char*, int>();
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

		delete this->keyBuffer;
		delete this->physicsModel;
	}


	bool GLContext::initContext(int argc, char** argv, displayFunc dFunc, reshapeFunc rFunc, motionFunc mFunc, mouseStateFunc msFunc, keyboardDownFunc kdFunc, keyboardUpFunc kuFunc, keyboardSpecialFunc ksFunc, keyboardSpecialUpFunc ksuFunc)
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
		glutInitWindowSize(1080, 720);
		glutInitContextVersion(4, 3);
		glutInitContextProfile(GLUT_FORWARD_COMPATIBLE);
		glutCreateWindow(argv[0]);
		
		glEnable(GL_DEPTH_TEST);

		glutIdleFunc(dFunc);
		glutReshapeFunc(rFunc);
		glutMotionFunc(mFunc);
		glutPassiveMotionFunc(mFunc);
		glutMouseFunc(msFunc);
		glutKeyboardFunc(kdFunc);
		glutKeyboardUpFunc(kuFunc);
		glutSpecialFunc(ksFunc);
		glutSpecialUpFunc(ksuFunc);

		if (glewInit()) {
			std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
			this->success = false;
			return false;
		}

		this->keyBuffer = new KeyBuffer();
		this->physicsModel = new PhysicsModel(9.81, 100.0);

		this->success = true;
		this->mouseMoved = false;
		this->mouseDown = false;
		this->mouseLastX = -1;
		this->mouseLastY = -1;
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

				// Gen texture

				GLuint tex;

				loadImageFromFile(tex, s[i].texture);

				std::pair<const char*, GLuint> texture(s[i].materialName, tex);
				std::pair<const char*, int> tiling(s[i].materialName, s[i].tiling);

				this->textures.insert(texture);
				this->tiling.insert(tiling);

				if (s[i].normalMap != NULL)
				{
					GLuint nMap;

					loadImageFromFile(nMap, s[i].normalMap);

					std::pair<const char*, GLuint> normalMap(s[i].materialName, nMap);
					this->normalMaps.insert(normalMap);
				}
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

			

			// End load texture


			for (int i = 0; i < n; i++)
			{
				GLfloat *texCoords = new GLfloat[72];

				int tiling = this->tiling[m[i].shader];

				for (int i = 0; i < 6; i++)
				{
					texCoords[i * 12] = m[i].texCoord0;
					texCoords[i * 12 + 1] = tiling * m[i].texCoord1;
					texCoords[i * 12 + 2] = m[i].texCoord0;
					texCoords[i * 12 + 3] = m[i].texCoord0;
					texCoords[i * 12 + 4] = tiling * m[i].texCoord1;
					texCoords[i * 12 + 5] = m[i].texCoord0;
					texCoords[i * 12 + 6] = m[i].texCoord0;
					texCoords[i * 12 + 7] = tiling * m[i].texCoord1;
					texCoords[i * 12 + 8] = tiling * m[i].texCoord1;
					texCoords[i * 12 + 9] = m[i].texCoord0;
					texCoords[i * 12 + 10] = tiling * m[i].texCoord1;
					texCoords[i * 12 + 11] = tiling * m[i].texCoord1;
				}

				int vertexCount = m[i].vertices.size();
				int normalCount = m[i].normals.size();

				glBindVertexArray(this->VAOs[i]);
				glBindBuffer(GL_ARRAY_BUFFER, this->VBOs[i]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (vertexCount + normalCount + 72), NULL, GL_STATIC_DRAW);

				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * vertexCount, &m[i].vertices.front());
				glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexCount, sizeof(GLfloat) * normalCount, &m[i].normals.front());
				glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (vertexCount + normalCount), sizeof(GLfloat) * 72, texCoords);

				delete texCoords;

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

				GLuint vTexCoord = glGetAttribLocation(program, "texcoord");

				glEnableVertexAttribArray(vTexCoord);
				glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(GLfloat) * (vertexCount + normalCount)));
				

				this->models[i] = m[i];
			}

			return true;
		}
		return false;
	}

	void GLContext::initCamera(glm::vec3 position, glm::vec3 gaze)
	{
		if (!this->cameraInitialized)
		{
			delete camera;
		}
		this->camera = new Camera(position, gaze);
		this->cameraInitialized = true;

	}

	void GLContext::initLight(glm::vec3 light)
	{
		this->light = light;
	}

	void GLContext::run()
	{
		if (this->success)
		{
			this->lastTime = glutGet(GLUT_ELAPSED_TIME);
			this->lightingEnabled = true;

			float width = glutGet(GLUT_WINDOW_WIDTH);
			float height = glutGet(GLUT_WINDOW_HEIGHT);
			float ratio = width / height;

			this->projectionTransform = glm::perspective<float>(45.0f, ratio, 0.01f, 1000.0f);
			this->viewTransform = glm::mat4() * glm::lookAt(this->camera->position, this->camera->gaze, glm::vec3(0.0, 1.0, 0.0));

			glutMainLoop();
		}
	}

	void GLContext::render()
	{
		if (this->success)
		{
			int currentTime = glutGet(GLUT_ELAPSED_TIME);
			int deltaTime = currentTime - this->lastTime;
			this->lastTime = currentTime;

			processKeyboardEvents();
			processPhysics(deltaTime);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.0, 0.0, 0.0, 1.0);

			for (int i = 0; i < this->numVAOs; i++)
			{
				model m = this->models[i];

				GLuint program = this->shaderPrograms[m.shader];
				glUseProgram(program);
				glBindVertexArray(this->VAOs[i]);

				GLint texLoc = glGetUniformLocation(program, "uTex");
				glUniform1i(texLoc, 0);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, this->textures[m.shader]);

				GLint normalMapEnabledLoc = glGetUniformLocation(program, "uNormalMapEnabled");

				if (this->normalMaps.count(m.shader) > 0)
				{
					GLint normalMapLoc = glGetUniformLocation(program, "uNormalMap");
					glUniform1i(texLoc, 1);

					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, this->normalMaps[m.shader]);

					GLint normalMapEnabledLoc = glGetUniformLocation(program, "uNormalMapEnabled");

					glUniform1i(normalMapEnabledLoc, 1);
				}
				else
				{
					glUniform1i(normalMapEnabledLoc, 0);
				}

				float theta = fmod(m.rotationTheta * currentTime, 360);

				glm::vec3 rotationTheta = glm::vec3(m.rotationAxis == DIRECTION::LEFT, m.rotationAxis == DIRECTION::UP, m.rotationAxis == DIRECTION::FRONT);

				glm::mat4 mod = glm::translate(glm::mat4(1.0) , glm::vec3(m.position.x, m.position.y, m.position.z));
				mod = glm::rotate(mod, theta, rotationTheta);

				GLint projectionTransformLoc = glGetUniformLocation(program, "projectionTransform");

				glUniformMatrix4fv(projectionTransformLoc, 1, GL_FALSE, glm::value_ptr(this->projectionTransform));

				GLint viewTransformLoc = glGetUniformLocation(program, "viewTransform");

				glUniformMatrix4fv(viewTransformLoc, 1, GL_FALSE, glm::value_ptr(this->viewTransform));

				GLint modelTransformLoc = glGetUniformLocation(program, "modelTransform");

				glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(mod));

				GLint lightLoc = glGetUniformLocation(program, "uLight");

				glUniform3fv(lightLoc, 1, &this->light[0]);

				GLint lightingEnabledLoc = glGetUniformLocation(program, "uLightingEnabled");

				glUniform1i(lightingEnabledLoc, this->lightingEnabled);

				glDrawArrays(this->models[i].renderType, 0, this->models[i].vertices.size() / VECTOR_SIZE);
			}

			glFlush();
			glutSwapBuffers();
		}
	}

	void GLContext::quit()
	{
		glutLeaveMainLoop();
	}

	void GLContext::reshape(int w, int h)
	{
		float width = glutGet(GLUT_WINDOW_WIDTH);
		float height = glutGet(GLUT_WINDOW_HEIGHT);
		float ratio = width / height;

		this->projectionTransform = glm::perspective<float>(45.0f, ratio, 0.01f, 1000.0f);
	}

	void GLContext::mouseStateChanged(int button, int state)
	{
		if (button == GLUT_RIGHT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				mouseDown = true;
			}
			else
			{
				mouseDown = false;
			}
		}
	}

	void GLContext::rotateCamera(float x, float y, float smoothing)
	{
		if (this->mouseLastX == -1 && this->mouseLastX == -1)
		{
			this->mouseLastX = x;
			this->mouseLastY = y;
		}

		float deltaX = (float)this->mouseLastX - x;
		float deltaY = (float)this->mouseLastY - y;

		this->mouseLastX = x;
		this->mouseLastY = y;

		if (this->mouseDown == false)
		{
			return;
		}

		deltaX *= smoothing;
		deltaY *= smoothing;

		this->camera->pitch(deltaY);
		this->camera->yaw(deltaX);


		this->viewTransform = glm::lookAt(this->camera->position, this->camera->gaze, this->camera->up);
	}

	void GLContext::moveCamera(Camera::DIRECTION direction)
	{
		this->camera->translate(direction);

		this->viewTransform = glm::lookAt(this->camera->position, this->camera->gaze, this->camera->up);
	}

	void GLContext::keyPressed(unsigned char key)
	{
		this->keyBuffer->set(key);

		if (key == 'q')
		{
			glutLeaveMainLoop();
		}
		if (key == ' ')
		{
			// temporarily disabled until issues are resolved
			//this->physicsModel->jump(JUMP_IMPULSE);
		}
		if (key == 't') // toggle
		{
			this->lightingEnabled = !this->lightingEnabled;
		}
	}

	void GLContext::keyReleased(unsigned char key)
	{
		this->keyBuffer->unset(key);
	}

	void GLContext::keySpecial(int key)
	{
		this->keyBuffer->setSpecial(key);
	}

	void GLContext::keySpecialUp(int key)
	{
		this->keyBuffer->unsetSpecial(key);
	}

	void GLContext::processKeyboardEvents()
	{
		if (this->keyBuffer->isSet('w'))
		{
			this->camera->translate(Camera::DIRECTION::FORWARD, 0.75);
		}
		if (this->keyBuffer->isSet('W'))
		{
			this->camera->translate(Camera::DIRECTION::FORWARD, 0.25f);
		}
		if (this->keyBuffer->isSet('a'))
		{
			this->camera->translate(Camera::DIRECTION::LEFT, 0.75);
		}
		if (this->keyBuffer->isSet('A'))
		{
			this->camera->translate(Camera::DIRECTION::LEFT, 0.25f);
		}
		if (this->keyBuffer->isSet('s'))
		{
			this->camera->translate(Camera::DIRECTION::BACK, 0.75);
		}
		if (this->keyBuffer->isSet('S'))
		{
			this->camera->translate(Camera::DIRECTION::BACK, 0.25f);
		}
		if (this->keyBuffer->isSet('d'))
		{
			this->camera->translate(Camera::DIRECTION::RIGHT, 0.75);
		}
		if (this->keyBuffer->isSet('D'))
		{
			this->camera->translate(Camera::DIRECTION::RIGHT, 0.25f);
		}
		if (this->keyBuffer->isSetSpecial(GLUT_KEY_LEFT))
		{
			this->camera->yaw(1.5f);
		}
		if (this->keyBuffer->isSetSpecial(GLUT_KEY_RIGHT))
		{
			this->camera->yaw(-1.5f);
		}
		if (this->keyBuffer->isSetSpecial(GLUT_KEY_UP))
		{
			this->camera->pitch(1.0f);
		}
		if (this->keyBuffer->isSetSpecial(GLUT_KEY_DOWN))
		{
			this->camera->pitch(-1.0f);
		}
		if (this->keyBuffer->isSet('j'))
		{
			this->light = this->light - this->camera->right * 0.25f;
		}
		if (this->keyBuffer->isSet('l'))
		{
			this->light = this->light + this->camera->right * 0.25f;
		}
		if (this->keyBuffer->isSet('i'))
		{
			glm::vec3 direction = this->camera->front;
			direction.y = 0;
			this->light = this->light + glm::normalize(direction) * 0.25f;
		}
		if (this->keyBuffer->isSet('k'))
		{
			glm::vec3 direction = this->camera->front;
			direction.y = 0;
			this->light = this->light - glm::normalize(direction) * 0.25f;
		}
		if (this->keyBuffer->isSet('['))
		{
			this->light = this->light + glm::vec3(0, 0.25f, 0);
		}
		if (this->keyBuffer->isSet(']'))
		{
			this->light = this->light - glm::vec3(0, 0.25f, 0);
		}

		this->viewTransform = glm::lookAt(this->camera->position, this->camera->gaze, this->camera->up);
	}

	void GLContext::processPhysics(float deltaTime)
	{
		if (!this->physicsModel->onGround && this->camera->position.y < HEIGHT_FROM_GROUND)
		{
			this->physicsModel->contactedGround();
			this->camera->position.y = HEIGHT_FROM_GROUND;
			this->viewTransform = glm::lookAt(this->camera->position, this->camera->gaze, this->camera->up);
			return;
		}

		this->camera->translate(glm::vec3(0.0, 1.0, 0.0), this->physicsModel->deltaY(deltaTime));


	}

	bool validAttribLocation(GLuint location)
	{
		int max_attribs;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_attribs);

		return location < (GLuint)max_attribs;
	}

	void loadImageFromFile(GLuint &loc, std::string filename)
	{
		glGenTextures(1, &loc);
		glBindTexture(GL_TEXTURE_2D, loc);

		std::string filepath = "Textures/";
		filepath += filename;

		int w;
		int h;
		int comp;
		unsigned char* image = stbi_load(filepath.c_str(), &w, &h, &comp, STBI_rgb_alpha);

		if (comp == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		else if (comp == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(image);
	}
}