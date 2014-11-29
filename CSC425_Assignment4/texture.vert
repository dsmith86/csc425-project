#version 430 core

in vec3 vPosition;
in vec3 vNormal;
in vec2 texcoord;

out vec4 color;
out vec2 Texcoord;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelTransform;
uniform vec3 uLight;

void main()
{
	gl_Position = projectionTransform * viewTransform * modelTransform * vec4(vPosition, 1.0);

	float lightIntensity = 1.0;
	float diffuseConstant = 0.1;

	float ambient = 0.1;
	float diffuse = diffuseConstant * dot(uLight, vNormal) * lightIntensity;

	float light = ambient + diffuse;

	color = vec4(1.0) * light;

	Texcoord = texcoord;
}