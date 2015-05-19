#version 150 core

#type vertex

#include <glr>

in vec3 in_Position;
in vec2 in_Texture;
in vec4 in_Color;
in vec3 in_Normal;

out vec4 color;

void main()
{
	gl_Position = pvmMatrix * vec4(in_Position, 1.0);
	
	color = in_Color;
}
