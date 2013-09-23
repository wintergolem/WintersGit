#version 330

// very simple vertex shader that just transforms an object-space
// position into clip space and passes along a texture coordinate

in vec4 position;
in vec2 UV;

out vec2 vUV;
out vec4 vColor;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform float Phase;


void main()
{
	vUV = UV;
	//gl_Position = Projection * View * Model * position;

	vec4 pulse = position;
	//pulse.y = position.y * sin(Phase);
	//vColor = vec4(0,sin(sin(position.y)),sin(position.z), 1);
	//vColor = vec4(0.3f,0.3f,0.3f,1);
	vUV = UV;
	gl_Position = Projection * View * Model * pulse;
}