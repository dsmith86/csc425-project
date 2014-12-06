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
	vec3 normal = (modelTransform * vec4(vNormal, 1.0)).xyz;
	vec3 light = uLight - (modelTransform * vec4(vPosition, 1.0)).xyz;

	float lightIntensity = 1.0;
	float diffuseConstant = 0.1;

	float ambient = 0.1;
	float diffuse = diffuseConstant * max(dot(light, normal), 0.0) * lightIntensity;

	float lightMultiplier = ambient + diffuse;

	color = vec4(1.0);

	Texcoord = texcoord;
}