#version 150 core

#ifndef NUM_LIGHTS
#define NUM_LIGHTS 1
#endif

#type vertex

#include <glr>
#include <light>
#include <material>

in vec3 in_Position;
in vec2 in_Texture;
in vec3 in_Normal;

out vec2 textureCoord;
out vec4 pass_Color;


//@bind texture0
uniform sampler2D texture;

//@bind texture0
uniform 
sampler2D 
texture2
;

@bind Light
layout(std140) 
uniform 
LightSources 
{
	LightSource lightSources[ NUM_LIGHTS ];
};

@bind Light2
layout(std140) uniform LightSources2 {
	LightSource lightSources2[ NUM_LIGHTS ];
};


@bind Light2
uniform LightSources3 
{
	LightSource lightSources3[ NUM_LIGHTS ];
};

@bind Material
Material material = Material(
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	0.995
);

@bind Color
Material color = Material (
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	0.995
);

@bind Color
Material color2 = Material (
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	0.995
)
;


void main() {
	gl_Position = pvmMatrix * vec4(in_Position, 1.0);
	
	textureCoord = in_Texture;
	
	vec3 normalDirection = normalize(normalMatrix * in_Normal);
	vec3 lightDirection = normalize(vec3(lightSources[0].direction));
	
	vec3 diffuseReflection = vec3(lightSources[0].diffuse) * vec3(material.diffuse) * max(0.0, dot(normalDirection, lightDirection));
	
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
	
	pass_Color = vec4(diffuseReflection, 1.0);
	//pass_Color = vec4((0.5 * normalDirection) + vec3(0.5), 1.0);
	//pass_Color = ambient;
}
