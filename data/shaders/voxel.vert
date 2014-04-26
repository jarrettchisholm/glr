#version 150 core

#ifndef NUM_LIGHTS
#define NUM_LIGHTS 1
#endif

#type vertex

#include <glr>
#include <light>

in vec3 in_Position;
in vec2 in_Texture;
in vec4 in_Color;
in vec3 in_Normal;
@location 6
in vec4 in_texBlend;

out vec2 textureCoord;
out vec3 normal;
out vec3 terrainTextureNormal;
out vec3 lightDirection;
out vec3 position;
out vec4 color;
out vec4 texBlend;

@bind Light
layout(std140) uniform Lights 
{
	Light lights[ NUM_LIGHTS ];
};

void main()
{
	gl_Position = pvmMatrix * vec4(in_Position, 1.0);
	
	// Assign texture coordinates
	textureCoord = in_Texture;
	
	// Calculate normal
	terrainTextureNormal = in_Normal;
	normal = normalize(normalMatrix * in_Normal);
	
	// Calculate light direction
	vec4 lightDirTemp = viewMatrix * lights[0].direction;
	lightDirection = normalize(vec3(lightDirTemp));
	
	color = in_Color;
	position = in_Position;
	texBlend = in_texBlend;
}
