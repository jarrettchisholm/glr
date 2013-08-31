#version 150 core

#ifndef NUM_LIGHTS
#define NUM_LIGHTS 1
#endif

#ifndef MAX_BONES
#define MAX_BONES 100
#endif

#type vertex

#include <glr>
#include <light>

in vec3 in_Position;
in vec2 in_Texture;
in vec3 in_Normal;
in vec2 in_Bone;

in uvec4 in_BoneIds;
in vec4 in_BoneWeights;

out vec2 textureCoord;
out vec3 normalDirection;
out vec3 lightDirection;

@bind Light
layout(std140) uniform Lights 
{
	Light lights[ NUM_LIGHTS ];
};

@bind Bone
layout(std140) uniform Bones 
{
	mat4 bones[ MAX_BONES ];
};

void main() {
	// Calculate the transformation on the vertex position based on the bone weightings
	mat4 boneTransform = bones[in_BoneIds[0]] * in_BoneWeights[0];
    boneTransform     += bones[in_BoneIds[1]] * in_BoneWeights[1];
    boneTransform     += bones[in_BoneIds[2]] * in_BoneWeights[2];
    boneTransform     += bones[in_BoneIds[3]] * in_BoneWeights[3];
    
    //boneTransform = bones[int(in_Bone[0])] * in_Bone[1];
    
    vec4 tempPosition = boneTransform * vec4(in_Position, 1.0);
    
	gl_Position = pvmMatrix * tempPosition;
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
