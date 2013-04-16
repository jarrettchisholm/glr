#version 150 core

#ifndef NUM_LIGHTS
#define NUM_LIGHTS 1
#endif

#type vertex

#include <glr>
#include <light>
//#include <material>

in vec3 in_Position;
in vec2 in_Texture;
in vec3 in_Normal;

out vec2 textureCoord;
out vec3 normalDirection;
out vec3 lightDirection;

//@bind texture0
//uniform sampler2DArray texture;

@bind Light
layout(std140) uniform Lights 
{
	Light lights[ NUM_LIGHTS ];
};

//@bind Material
/*
Material material = Material(
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	0.995,
	0.995
);
*/

void main() {
	gl_Position = pvmMatrix * vec4(in_Position, 1.0);
	
	textureCoord = in_Texture;

	normalDirection = normalize(normalMatrix * in_Normal);
	lightDirection = normalize(vec3(lights[0].direction));
	
	//vec3 diffuseReflection = vec3(lights[0].diffuse) * vec3(material.diffuse) * max(0.0, dot(normalDirection, lightDirection));
	
	/*
	float bug = 0.0;	
	bvec3 result = equal( diffuseReflection, vec3(0.0, 0.0, 0.0) );
	if(result[0] && result[1] && result[2]) bug = 1.0;
	diffuseReflection.x += bug;
	*/
	
	/*
	float bug2 = 0.0;
	bool result2 = dot(normalDirection, lightDirection) > 0.0;
	if(result2) bug2 = 1.0;
	diffuseReflection.x += bug2;
	*/
}
