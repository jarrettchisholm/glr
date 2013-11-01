#version 150 core

#type fragment

in vec2 textureCoord;
in vec3 normalDirection;
in vec3 lightDirection;
in vec4 color;
in float bug;

void main() {
	gl_FragColor = color;
}
