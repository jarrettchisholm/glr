#version 150 core

#type vertex

#include <glr>

in vec3 in_Position;
in vec2 in_Texture;
in vec4 in_Color;
in vec3 in_Normal;

out vec2 textureCoord;
out vec4 color;

void main()
{
	gl_Position = pvmMatrix * vec4(in_Position, 1.0);
	
	// Assign texture coordinates
	textureCoord = in_Texture;
	
	color = in_Color;
}
