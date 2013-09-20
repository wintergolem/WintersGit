#version 330

// very simple pixel shader that just samples a texture

in vec2 vUV;

out vec4 outColour;

uniform sampler2D diffuseTexture;
uniform sampler2D diffuseTextureTwo;

void main()
{
	vec4 day = texture2D(diffuseTexture, vUV);
	vec4 night = texture2D(diffuseTextureTwo, vUV);
    outColour = night;
	sfd
	//outColour = day + night;
}
