#version 150 core

#ifndef NUM_LIGHTS
#define NUM_LIGHTS 1
#endif

#type vertex

#include <glr>
#include <light>

in vec3 in_Position;
in vec2 in_Texture;
in vec3 in_Normal;
in vec2 in_Bone;

out vec2 textureCoord;
out vec3 normalDirection;
out vec3 lightDirection;

@bind Light
layout(std140) uniform Lights 
{
	Light lights[ NUM_LIGHTS ];
};

void main() {
	gl_Position = pvmMatrix * vec4(in_Position, 1.0);
	
	vec4 lightDirTemp = viewMatrix * lights[0].direction;
	
	textureCoord = in_Texture;

	normalDirection = normalize(normalMatrix * in_Normal);
	lightDirection = normalize(vec3(lightDirTemp));
	
	/*
	float bug = 0.0;	
	bvec3 result = equal( diffuseReflection, vec3(0.0, 0.0, 0.0) );
	if(result[0] && result[1] && result[2]) bug = 1.0;
	diffuseReflection.x += bug;
	*/
}
