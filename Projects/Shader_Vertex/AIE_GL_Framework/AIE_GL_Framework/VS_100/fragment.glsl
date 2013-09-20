#version 430

// very simple fragment


in vec4 vColor;
in vec4 position;

out vec4 outColor;

void main()
{
	outColor = vColor*vec4(.3*(position.y),.5,.1,1);
	
}
