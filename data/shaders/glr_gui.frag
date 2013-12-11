#version 150 core

#extension GL_EXT_texture_array : enable

#type fragment

in vec2 textureCoord;
in vec3 normalDirection;

@bind Texture2D
uniform sampler2D tex2D;

void main()
{
	
	gl_FragColor = texture2D(tex2D, textureCoord);
	
	/*
	float bug = 0.0;	
	bvec4 result = equal( materials[0].diffuse, vec4(0.0, 0.0, 0.0, 0.0) );
	if(result[0] && result[1] && result[2]) bug = 1.0;
	gl_FragColor.x += bug;
	*/
}
