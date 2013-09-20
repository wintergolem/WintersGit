#version 330

// very simple vertex shader that just transforms an object-space
// position into clip space and passes along a texture coordinate

in vec4 Position;
in vec4 color;
in vec2 UV;

out vec2 vUV;
out vec4 vColor;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform float Phase;

void main()
{
	//vec4 pulse = vec4(1,1,1,1);
	vec4 pulse = Position;
	pulse.y = (1* (1*sin(Phase + Position.x/1))+ (1*sin(Phase + Position.z/1)));//
	vColor = vec4(0,sin(sin(Position.y)),sin(Position.z), 1);
	vUV = UV;
	gl_Position = Projection * View * Model * pulse;
}