#version 400 

// very simple vertex shader that just transforms an object-space
// position into clip space and passes along a texture coordinate

in vec4 vPosition;
in vec4 inColor;
in vec2 UV;

out vec2 vUV;
out vec4 vColor;


uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

uniform float Phase;
uniform float Amplitude;

void main()
{
	vec4 v = vPosition;
	
	v.y = (sin(Phase + (v.x/5) ) + sin(Phase + (v.z/4))) * Amplitude ;
	vUV = UV;
	gl_Position = Projection * View * Model * v;
	
	vColor = vec4(0.0,0.0,v.y,1.0);
}
