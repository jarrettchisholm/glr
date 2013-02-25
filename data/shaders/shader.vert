#version 150 core

#define NUM_LIGHTS 1

#type vertex

#include <oglre>
#include <light>
#include <material>

in vec3 in_Position;
in vec2 in_Texture;
in vec3 in_Normal;

out vec2 textureCoord;
out vec4 pass_Color;

/*
@bind Light
uniform LightSources {
	LightSource lightSources[ NUM_LIGHTS ];
};
*/

uniform LightSources {
	vec4 ambient = vec4(0.5, 0.5, 0.5, 1.0);
	vec4 diffuse = vec4(0.5, 0.5, 0.5, 1.0);
	vec4 specular = vec4(0.5, 0.5, 0.5, 1.0);
	vec4 position = vec4(1.5, 7, 0.5, 1.0);
	vec4 direction = vec4(0.0, 0.0, 0.0, 1.0);
} lightSources;

//@bind Material
Material mymaterial = Material(
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	0.995
);


void main() {
	gl_Position = pvmMatrix * vec4(in_Position, 1.0);
	
	textureCoord = in_Texture;
	
	
	vec3 normalDirection = normalize(normalMatrix * in_Normal);
	vec3 lightDirection = normalize(vec3(lightSources.direction));
	
	vec3 diffuseReflection = vec3(lightSources.diffuse) * vec3(mymaterial.diffuse) * max(0.0, dot(normalDirection, lightDirection));
	
	/*
	float bug = 0.0;
	bvec3 result = equal( diffuseReflection, vec3(0.0, 0.0, 0.0) );
	if(result[0]) bug = 1.0;
	
	diffuseReflection.x += bug;
	*/
	
	pass_Color = vec4(diffuseReflection, 1.0);
}
