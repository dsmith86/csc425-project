#include <unordered_map>
#include <vector>
#include "vgl.h"
#include "Camera.h"
#include "InstancedMeshFactory.h"
#include <vec3.hpp>
#include <glm.hpp>

namespace GLContext {

#define VECTOR_SIZE 3

	typedef void(*displayFunc)(void);
	typedef void(*reshapeFunc)(int w, int h);

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
	};

	typedef glm::vec4 color4;

#pragma once
	class GLContext
	{
	public:
		GLContext();
		~GLContext();
		bool initContext(int argc, char** argv, displayFunc dFunc, reshapeFunc rFunc);
		bool initShaders(const material s[], int n);
		bool initModels(const std::vector<InstancedMeshFactory> m);
		void initCamera(vec3 position, vec3 gaze);
		void initLight(float x, float y, float z);
		void run();
		void render();
		void reshape(int w, int h);
	private:
		Camera *camera;
		bool cameraInitialized;
		vec3 light;
		bool success;
		size_t numVAOs;
		size_t numVBOs;
		GLuint *VAOs;
		GLuint *VBOs;
		InstancedMeshFactory *meshes;
		std::unordered_map<const char*, GLuint> shaderPrograms;
		float w;
		float h;

		glm::mat4 projectionTransform;
		glm::mat4 viewTransform;
	};

}
