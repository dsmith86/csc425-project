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
    gl_Position = projection * view * model * vec4(vPosition, 1.0);

	color = uColor;

}