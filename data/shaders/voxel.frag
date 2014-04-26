#version 150 core

#extension GL_EXT_texture_array : enable

#ifndef NUM_MATERIALS
#define NUM_MATERIALS 1
#endif

#type fragment

#include <material>

in vec2 textureCoord;
in vec3 normal;
in vec3 terrainTextureNormal;
in vec3 lightDirection;
in vec3 position;
in vec4 color;
in vec4 texBlend;

@bind Texture2DArray
uniform sampler2DArray tex2DArray;

@bind Material
layout(std140) uniform Materials 
{
	Material materials[ NUM_MATERIALS ];
};


void main()
{
	vec3 ct, cf;
	vec4 texel;
	float intensity, at, af;
	intensity = max( dot(lightDirection, normalize(normal)), 0.0 );
 
	cf = (intensity * materials[0].diffuse.rgb) + materials[0].ambient.rgb;
	af = materials[0].diffuse.a;
	//texel = texture2DArray(tex2DArray, vec3(textureCoord, 1));



	// Testing tri-planar projection 
	float textureScale = 0.2;
 
	vec2 coord1 = position.yz * textureScale;
	vec2 coord2 = position.zx * textureScale;
	vec2 coord3 = position.xy * textureScale;
	
	float nx = abs( terrainTextureNormal.x );
	float ny = abs( terrainTextureNormal.y );
	float nz = abs( terrainTextureNormal.z );
	
	// Pick the projection that is best, based on the normal
	if (nx > ny && nx > nz)
	{
		vec4 sample1 = texture2DArray(tex2DArray, vec3(coord1, 0)) * texBlend.x;
		vec4 sample2 = texture2DArray(tex2DArray, vec3(coord1, 1)) * texBlend.y;
		texel = sample1 + sample2;
		//texel = texture2DArray(tex2DArray, vec3(coord1, 0));
	}
	else if (ny > nx && ny > nz)
	{
		vec4 sample1 = texture2DArray(tex2DArray, vec3(coord2, 0)) * texBlend.x;
		vec4 sample2 = texture2DArray(tex2DArray, vec3(coord2, 1)) * texBlend.y;
		texel = sample1 + sample2;
		//texel = texture2DArray(tex2DArray, vec3(coord2, 0));
	}
	else if (nz > nx && nz > ny)
	{
		vec4 sample1 = texture2DArray(tex2DArray, vec3(coord3, 0)) * texBlend.x;
		vec4 sample2 = texture2DArray(tex2DArray, vec3(coord3, 1)) * texBlend.y;
		texel = sample1 + sample2;
		//texel = texture2DArray(tex2DArray, vec3(coord3, 0));
	}
	else
	{
		// TODO: I'm not sure this is working properly - and if it is, I'm not sure I like it...
		// If there is no clear winner, then just blend
		vec3 blendWeights = abs( terrainTextureNormal ); // Tighten up the blending zone:
		//blendWeights = (blendWeights - 0.2) * 7.0;
		blendWeights = (blendWeights - 0.2);
		blendWeights = max(blendWeights, 0.0);
		// Force weights to sum to 1.0 (very important!)
		blendWeights /= (blendWeights.x + blendWeights.y + blendWeights.z );
		
		// Take the three texture samples
		// Project along x axis
		vec4 sampleX1 = texture2DArray(tex2DArray, vec3(coord1, 0)) * texBlend.x;
		vec4 sampleX2 = texture2DArray(tex2DArray, vec3(coord1, 1)) * texBlend.y;
		
		// Project along y axis
		vec4 sampleY1 = texture2DArray(tex2DArray, vec3(coord2, 0)) * texBlend.x;
		vec4 sampleY2 = texture2DArray(tex2DArray, vec3(coord2, 1)) * texBlend.y;
		
		// Project along z axis
		vec4 sampleZ1 = texture2DArray(tex2DArray, vec3(coord3, 0)) * texBlend.x;
		vec4 sampleZ2 = texture2DArray(tex2DArray, vec3(coord3, 1)) * texBlend.y;
		
		vec4 sampleX = sampleX1 + sampleX2;
		vec4 sampleY = sampleY1 + sampleY2;
		vec4 sampleZ = sampleZ1 + sampleZ2;
		
		//sampleX = texture2DArray(tex2DArray, vec3(coord1, 0));
		//sampleY = texture2DArray(tex2DArray, vec3(coord2, 0));
		//sampleZ = texture2DArray(tex2DArray, vec3(coord3, 0));
		
		texel = sampleX * blendWeights.x + sampleY * blendWeights.y + sampleZ * blendWeights.z;
	}
	
	// Blend the samples according to the normal
	//vec4 blendedColour = sampleX * blendWeights.x + sampleY * blendWeights.y + sampleZ * blendWeights.z;
 
	ct = intensity * texel.rgb;
	at = texel.a;
	//ct = color.xyz;
	//at = color.a;

	gl_FragColor = vec4(ct*cf, at*af);
	//gl_FragColor = vec4(ct, at);
	
	// Error check
	//if ( blendWeights.x + blendWeights.y + blendWeights.z > 1.02f || blendWeights.x + blendWeights.y + blendWeights.z < 0.98f)
	//	gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
