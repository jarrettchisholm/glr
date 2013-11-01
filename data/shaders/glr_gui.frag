#version 150 core

#extension GL_EXT_texture_array : enable

#type fragment

in vec2 textureCoord;
in vec3 normalDirection;

@bind Texture
uniform sampler2DArray tex;

void main()
{
	
	gl_FragColor = texture2DArray(tex, vec3(textureCoord, 1));
	
	/*
	float bug = 0.0;	
	bvec4 result = equal( materials[0].diffuse, vec4(0.0, 0.0, 0.0, 0.0) );
	if(result[0] && result[1] && result[2]) bug = 1.0;
	gl_FragColor.x += bug;
	*/
}
