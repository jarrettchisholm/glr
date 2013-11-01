#version 150 core

#type vertex

#include <glr>

in vec3 in_Position;
in vec2 in_Texture;
in vec3 in_Normal;

out vec2 textureCoord;
out vec3 normalDirection;
out vec3 lightDirection;

void main()
{
	gl_Position = pvmMatrix * vec4(in_Position, 1.0);
	
	textureCoord = in_Texture;

	normalDirection = normalize(normalMatrix * in_Normal);
	
	/*
	float bug = 0.0;	
	bvec3 result = equal( diffuseReflection, vec3(0.0, 0.0, 0.0) );
	if(result[0] && result[1] && result[2]) bug = 1.0;
	diffuseReflection.x += bug;
	*/
}
