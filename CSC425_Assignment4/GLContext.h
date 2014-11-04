#include <unordered_map>
#include <vector>
#include "vgl.h"
using namespace std;

#define VECTOR_SIZE 3

typedef void(*displayFunc)(void);

struct material
{
	const char *materialName;
	const char *vertexShader;
	const char *fragmentShader;
};

struct vec3
{
	float x;
	float y;
	float z;
};

struct model
{
	vector<GLfloat> vertices;
	vec3 color;
	const char *shader;
};

#pragma once
class GLContext
{
public:
	GLContext();
	~GLContext();
	bool initContext(int argc, char** argv, displayFunc dFunc);
	bool initShaders(const material s[], int n);
	bool initModels(const model m[], int n);
	void run();
	void render();
private:
	bool success;
	size_t numVAOs;
	size_t numVBOs;
	GLuint *VAOs;
	GLuint *VBOs;
	model *models;
	unordered_map<const char*, GLuint> shaderPrograms;
};

