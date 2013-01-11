#version 150 core

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

in vec3 in_Position;
in vec2 in_Texture;
in vec3 in_Color;

out vec2 textureCoord;
out vec3 pass_Color;

void main() {
	// gl_Position is pre-defined
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
	
	textureCoord = in_Texture;
	pass_Color = in_Color;
}
