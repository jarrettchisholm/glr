#version 150 core

#extension GL_EXT_texture_array : enable

#ifndef NUM_MATERIALS
#define NUM_MATERIALS 1
#endif

#type fragment

#include <material>

in vec2 textureCoord;
in vec3 normalDirection;
in vec3 lightDirection;
in float bug;

@bind texture0
uniform sampler2DArray tex;

@bind Material
layout(std140) uniform Materials 
{
	Material materials[ NUM_MATERIALS ];
};


void main() {
	vec3 ct, cf;
	vec4 texel;
	float intensity, at, af;
	intensity = max( dot(lightDirection, normalize(normalDirection)), 0.0 );
 
	cf = intensity * (materials[0].diffuse).rgb + materials[0].ambient.rgb;
	af = materials[0].diffuse.a;
	texel = texture2DArray(tex, vec3(textureCoord, 1));
 
	ct = texel.rgb;
	at = texel.a;
	
	gl_FragColor = vec4(ct * cf, at * af);
	
	
	// Show bugs
	if (bug != 0.0)
	{
		if (bug == 1.0)	
			gl_FragColor.x += bug;
		else if (bug == 2.0)	
			gl_FragColor.y += bug;
	}
	
	/*
	float bug = 0.0;	
	bvec4 result = equal( materials[0].diffuse, vec4(0.0, 0.0, 0.0, 0.0) );
	if(result[0] && result[1] && result[2]) bug = 1.0;
	gl_FragColor.x += bug;
	*/
}
