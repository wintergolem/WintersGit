#version 330

// very simple vertex shader that just transforms an object-space
// position into clip space and passes along a texture coordinate

layout(location = 0) in vec2 VertexPosition;

void main()
{
	gl_Position = vec4(VertexPosition, 0.0, 1.0);
}