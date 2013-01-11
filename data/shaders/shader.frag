#version 150 core

uniform sampler2D texture;

in vec3 pass_Color;
in vec2 textureCoord;

void main() {	
	vec4 out_Color = texture2D(texture, textureCoord);
	
	// gl_FragColor is pre-defined
	gl_FragColor = out_Color;
}
