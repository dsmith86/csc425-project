#version 430 core

in vec3 vPosition;
in vec3 vNormal;
in vec2 texcoord;

out vec4 color;
out vec2 Texcoord;
out vec3 position;
out vec3 normal;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelTransform;

void main()
{
	gl_Position = projectionTransform * viewTransform * modelTransform * vec4(vPosition, 1.0);
	position = (modelTransform * vec4(vPosition, 1.0)).xyz;
	normal = vNormal;

	color = vec4(1.0);

	Texcoord = texcoord;
}