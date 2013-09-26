#version 330

layout( quads, equal_spacing, ccw ) in;

uniform mat4 MVP; // projection * view * model

void main()
{
	//The tessellation u coordinate
	float u = gl_TessCoord.x;
	float v = gl_TessCorrd.y;

	//the quad vertices
	vec3 p0 = gl_in[0].gl_Position.xyz;
	vec3 p1 = gl_in[1].gl_Position.xyz;
	vec3 p2 = gl_in[2].gl_Position.xyz;
	vec3 p3 = gl_in[3].gl_Position.xyz;

	//Linear interpolation
	gl_Position =
		p0 * (1-u) * (1-v) +
		p1 * u * (1-v) +
		p3 * v * (1-u) +
		p2 * u * v;

	gl_Position = MVP * gl_Position;
}