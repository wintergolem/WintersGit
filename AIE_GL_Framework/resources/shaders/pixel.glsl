#version 330

// very simple pixel shader that just samples a texture

in vec2 vUV;

out vec4 outColour;

uniform sampler2D diffuseTexture;

void main()
{
    outColour = texture2D(de, vUV);
}
