#version 430 core

in vec3 vPosition;
in vec3 vNormal;
out vec4 color;

uniform mat4 projection, view, model;

uniform vec4 LightPosition;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;

uniform vec4 uColor;

void main()
{
	vec4 vertex_position = vec4(vPosition, 1.0);

    gl_Position = projection * view * model * vertex_position;

	vec3 pos = (model * view * vertex_position).xyz;

	vec3 L = normalize(LightPosition.xyz - pos);
	vec3 E = normalize(-pos);
	vec3 H = normalize(L + E);

	vec3 N = normalize(vec4(vNormal, 0.0)).xyz;

	vec4 ambient = vec4(0.1 * uColor.x, 0.1 * uColor.y, 0.1 * uColor.z, 1.0);

	float Kd = max(dot(L, N), 0.0);
	vec4 diffuse = Kd * uColor * 2;

	float Ks = max(dot(N, H), 0.0);
	vec4 specular = Ks * uColor * Shininess;

	color = ambient + diffuse + specular;
}