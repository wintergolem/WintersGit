#version 330

// very simple pixel shader that just samples a texture

in vec2 vUV;

out vec4 outColour;

uniform sampler2D diffuseTexture;
uniform sampler2D diffuseTextureTwo;
uniform float phase;

void main()
{
	vec4 day = texture(diffuseTexture, vUV);
	vec4 night = texture(diffuseTextureTwo, vUV);
	vec4 timeOfDay = abs(day*sin(phase) ) + abs( night* cos(phase) );
    outColour = timeOfDay;
}
