#version 430 core

in vec4 color;
in vec2 Texcoord;
in vec3 position;
in vec3 normal;

out vec4 fColor;

uniform sampler2D uTex;
uniform sampler2D uNormalMap;
uniform vec3 uLight;
uniform bool uLightingEnabled;
uniform bool uNormalMapEnabled;

void main()
{
	float lightMultiplier = 1;

	if (uLightingEnabled)
	{
		vec3 light = uLight - position;
		float lightDistance = length(light);
		light = normalize(light);

		float lightIntensity = 30.0;
		float diffuseConstant = 0.1;

		float ambient = 0.05;
		float diffuse = diffuseConstant * max(dot(light, normal), 0.0) * lightIntensity / lightDistance;

		lightMultiplier = ambient + diffuse;
	}

	fColor = texture(uTex, Texcoord) * color * lightMultiplier;

	
}