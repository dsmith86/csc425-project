#version 430 core

in vec3 vPosition;
in vec2 texcoord;

out vec4 color;
out vec2 Texcoord;

uniform mat4 projection, view, model;

void main()
{
	gl_Position = projection * view * model * vec4(vPosition, 1.0);

	color = vec4(vPosition * 2, 1.0);

	Texcoord = texcoord;
}