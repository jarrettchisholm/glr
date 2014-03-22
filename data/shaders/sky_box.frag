#version 150 core

#extension GL_EXT_texture_array : enable

#ifndef NUM_MATERIALS
#define NUM_MATERIALS 1
#endif

#type fragment

#include <material>

in vec2 textureCoord;
in vec4 color;

@bind Texture2D
uniform sampler2D tex2D;

@bind Material
layout(std140) uniform Materials 
{
	Material materials[ NUM_MATERIALS ];
};


void main()
{
	vec3 ct, cf;
	vec4 texel;
	float at, af;
 
	cf = (materials[0].diffuse).rgb + materials[0].ambient.rgb;
	af = materials[0].diffuse.a;
	texel = texture2D(tex2D, textureCoord);

	ct = texel.rgb;
	at = texel.a;

	gl_FragColor = vec4(ct * cf, at * af);
}
