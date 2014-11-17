#version 430 core

in vec3 vPosition;
out vec4 color;

uniform mat4 projection, view, model;

void main()
{
	gl_Position = projection * view * model * vec4(vPosition, 1.0);

	color = vec4(1.0, 1.0, 1.0, 1.0);
}