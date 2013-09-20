#version 430

// very simple pixel shader that just samples a texture

in vec2 vUV;
in vec4 Position;

out vec4 outColor;

uniform sampler2D diffuseTexture;
uniform sampler2D lightTexture;

uniform float Phase;

void main()
{
	vec4 Color1 = texture2D(diffuseTexture,vUV);
	vec4 Color2 = texture2D(lightTexture,vUV);

	//this will blend between the 2 textures
	//Color1 = Color1 * sin(Phase);

	


		outColor = Color2 + Color1;

}