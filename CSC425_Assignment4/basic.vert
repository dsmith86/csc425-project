#version 430 core

layout(location = 0) in vec4 vPosition;

uniform mat4 vModel;

void main()
{
	gl_Position = vModel * vPosition;
}