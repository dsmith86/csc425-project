#version 430 core

in vec4 color;
in vec2 Texcoord;

out vec4 fColor;

uniform sampler2D tex;

void main()
{
	fColor = texture(tex, Texcoord) * color;
}