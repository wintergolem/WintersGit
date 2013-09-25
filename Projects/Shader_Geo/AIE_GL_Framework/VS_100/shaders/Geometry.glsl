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
uniform float Phase;

void main()
{
	float n = gl_InvocationID;
	vec4 dir = vec4( cross( gl_in[1].gl_Position.xyz - gl_in[2].gl_Position.xyz,
							gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz), 0);

	dir = normalize(dir);

	float greyShade = fract( gl_PrimitiveID / 255.0f );

	for( int i = 0; i < 3 ; ++i)
	{
		if(n == 0) gColor = vec4(0,0,1,0);
		//if(mod(n, 5) == 0) gColor = vec4(0,0,1,0);
		else if(n != 0) gColor = vec4(0,1,0,0);
		else if(mod(n, 4) == 0) gColor = vec4(1,0,0,0);
		else if(mod(n, 3) == 0) gColor = vec4(1,0,1,0);
		else gColor = vec4(1,1,1,0);
		//gColor = vColor[1];
		vec4 position = (Projection * View * Model)* (gl_in[i].gl_Position  );//(Projection * View * Model);
		gl_Position = vec4( position.x+dir.x *Phase, position.y +dir.y*Phase , position.z +dir.z*Phase , position.w + dir.w*Phase); //* vec4(1,1,1,1);
		//gl_Position = vec4( position.x+dir.x , position.y +dir.y , position.z +dir.z , position.w + dir.w);
		//gl_Position = vec4( position.x, position.y , position.z , position.w); //* vec4(1,1,1,1);
		
		gUV = vUV[1];
		EmitVertex();
		
	}
	EndPrimitive();
}