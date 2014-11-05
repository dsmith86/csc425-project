#version 430 core

in vec3 vPosition;

uniform mat4 projection, view, model;

uniform vec4 uColor;

out vec4 color;

void main()
{
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
	color = uColor;
}