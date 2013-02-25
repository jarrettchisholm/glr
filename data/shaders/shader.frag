#version 150 core

#type fragment

@bind texture0
uniform sampler2D texture;

in vec4 pass_Color;
in vec2 textureCoord;

void main() {	
	vec4 out_Color = texture2D(texture, textureCoord);
	
	gl_FragColor = pass_Color;
	
	//gl_FragColor = out_Color;
}
