#version 330

// very simple pixel shader that just samples a texture

in vec4 gColor;
in vec2 gUV;

out vec4 outColor;

uniform sampler2D diffuseTexture;

void main()
{
	outColor = texture(diffuseTexture, gUV);
}