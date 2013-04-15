#version 150 core

#extension GL_EXT_texture_array : enable

#type fragment

#include <material>

@bind texture0
uniform sampler2DArray tex;

in vec4 pass_Color;
in vec2 textureCoord;
in vec3 normalDirection;
in vec3 lightDirection;

Material material = Material(
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	vec4(1.0, 0.8, 0.8, 1.0),
	0.995
);

void main() {	
	//vec4 out_Color = texture2D(texture, textureCoord);
	
	//gl_FragColor = pass_Color;
	
	//gl_FragColor = out_Color;
	
	vec3 ct, cf;
	vec4 texel;
	float intensity, at, af;
	intensity = max( dot(lightDirection, normalize(normalDirection)), 0.0 );
 
	cf = intensity * (material.diffuse).rgb + material.ambient.rgb;
	af = material.diffuse.a;
	texel = texture2DArray(tex, vec3(textureCoord, 1));
 
	ct = texel.rgb;
	at = texel.a;
	
	gl_FragColor = vec4(ct * cf, at * af) + pass_Color;
}
