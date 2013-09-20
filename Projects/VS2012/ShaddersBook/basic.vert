#version 400

in vec3 vertexPosition;
in vec3 vertexColor;

out vec3 vColor;

uniform mat4 RotationMatrix;

void main()
{
	vColor = VertexColor;
	gl_Position = vec4( vertexPosition, 1.0 );
}