#version 430 core

in vec3 vPosition;
in vec2 vTexCoord;
in vec3 iOffset;
in vec3 iRotation;

out vec4 color;
out vec2 Texcoord;

uniform mat4 perspectiveTransform;
uniform float deltaTime;

void main()
{
	gl_Position = perspectiveTransform * vec4(vPosition + iOffset, 1.0);

	color = vec4(vPosition * 2, 1.0);

	Texcoord = vTexCoord;
}