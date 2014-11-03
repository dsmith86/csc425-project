#include <unordered_map>
#include "vgl.h"
using namespace std;

struct shaderType
{
	const char *shaderName;
	const char *vertexShader;
	const char *fragmentShader;
};

#pragma once
class GLContext
{
public:
	GLContext();
	~GLContext();
	bool initContext(int argc, char** argv);
	bool initShaders(const shaderType s[], int n);
private:
	bool success;
	unordered_map<const char*, GLuint> shaderPrograms;
};

