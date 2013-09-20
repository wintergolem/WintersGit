#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 vUV[];
in vec4 vColor[];

out vec2 gUV;
out vec4 gColor;

uniform float Time;
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
void main()
{
	vec4 dir = vec4( cross( gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz,
							gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz), 0);

	dir = normalize(dir);

	float greyShade = fract( gl_PrimitiveID / 255.0f );

	for( int i = 0; i < 3 ; ++i)
	{
		if(i == 1)
		{
			gColor = vec4(1, greyShade, greyShade, 1);
			vec4 position = (Projection * View * Model)* (gl_in[i].gl_Position + dir * Time);//(Projection * View * Model);
			gl_Position = vec4(position.x, position.y+5*Time ,position.z, position.w); //* vec4(1,1,1,1);
		
			gUV = vUV[1];
			EmitVertex();
		}
		else if (i == 2)
		{
			gColor = vec4(greyShade, 1, greyShade, 1);
			vec4 position = (Projection * View * Model)* (gl_in[i].gl_Position + dir * Time);//(Projection * View * Model);
			gl_Position = vec4(position.x +5*Time, position.y ,position.z, position.w); //* vec4(1,1,1,1);
		
			gUV = vUV[1];
			EmitVertex();
		}
		else
		{
			gColor =vec4(greyShade, greyShade, 1, 1);
			vec4 position = (Projection * View * Model)* (gl_in[i].gl_Position + dir * Time);//(Projection * View * Model);
			gl_Position = vec4(position.x, position.y+2*Time ,position.z, position.w); //* vec4(1,1,1,1);
		
			gUV = vUV[1];
			EmitVertex();
		}
		
	}
	EndPrimitive();
}