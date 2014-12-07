#include <unordered_map>
#include <vector>
#include "vgl.h"
#include "Camera.h"
#include "KeyBuffer.h"
#include "PhysicsModel.h"
#include <vec3.hpp>
#include <glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)

#define JUMP_IMPULSE 50000.0f
#define HEIGHT_FROM_GROUND 1

namespace GLContext {

#define VECTOR_SIZE 3

	typedef void(*displayFunc)(void);
	typedef void(*reshapeFunc)(int w, int h);
	typedef void(*motionFunc)(int x, int y);
	typedef void(*mouseStateFunc)(int button, int state, int x, int y);
	typedef void(*keyboardDownFunc)(unsigned char key, int x, int y);
	typedef void(*keyboardUpFunc)(unsigned char key, int x, int y);
	typedef void(*keyboardSpecialFunc)(int key, int x, int y);
	typedef void(*keyboardSpecialUpFunc)(int key, int x, int y);

	typedef enum {
		UP, LEFT, FRONT
	} DIRECTION;

	struct vec3
	{
		float x;
		float y;
		float z;
	};

	struct material
	{
		const char *materialName;
		const char *vertexShader;
		const char *fragmentShader;
		const char *texture;
		const char *normalMap;
		int tiling;
	};

	struct model
	{
		std::vector<GLfloat> vertices;
		std::vector<GLfloat> normals;
		vec3 position;
		DIRECTION rotationAxis;
		float rotationTheta;
		vec3 color;
		const char *shader;
		GLenum renderType;
		GLfloat shininess;
		float texCoord0;
		float texCoord1;
	};

	typedef glm::vec4 color4;

#pragma once
	class GLContext
	{
	public:
		GLContext();
		~GLContext();
		bool initContext(int argc, char** argv,
			displayFunc dFunc,
			reshapeFunc rFunc,
			motionFunc mFunc,
			mouseStateFunc msFunc,
			keyboardDownFunc kdFunc,
			keyboardUpFunc kuFunc,
			keyboardSpecialFunc ksFunc,
			keyboardSpecialUpFunc ksuFunc);
		bool initShaders(const material s[], int n);
		bool initModels(const model m[], int n);
		void initCamera(glm::vec3 position, glm::vec3 gaze);
		void initLight(glm::vec3 light);
		void run();
		void render();
		void quit();
		void reshape(int w, int h);
		void keyPressed(unsigned char key);
		void keyReleased(unsigned char key);
		void keySpecial(int key);
		void keySpecialUp(int key);
		void mouseStateChanged(int button, int state);
		void rotateCamera(float x, float y, float smoothing);
		void moveCamera(Camera::DIRECTION direction);
	private:
		void processKeyboardEvents();
		void processPhysics(float deltaTime);
		Camera *camera;
		KeyBuffer *keyBuffer;
		PhysicsModel *physicsModel;
		bool cameraInitialized;
		glm::vec3 light;
		bool success;
		int lastTime;
		size_t numVAOs;
		size_t numVBOs;
		GLuint *VAOs;
		GLuint *VBOs;
		model *models;
		std::unordered_map<const char*, GLuint> shaderPrograms;
		std::unordered_map<const char*, GLuint> textures;
		std::unordered_map<const char*, GLuint> normalMaps;
		std::unordered_map<const char*, int> tiling;
		bool mouseMoved;
		bool mouseDown;
		bool lightingEnabled;
		int mouseLastX;
		int mouseLastY;

		glm::mat4 projectionTransform;
		glm::mat4 viewTransform;
	};

}
