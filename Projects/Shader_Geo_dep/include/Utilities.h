//////////////////////////////////////////////////////////////////////////
// Author:	Conan Bourke
// Date:	December 4 2012
// Brief:	Utility functions
//////////////////////////////////////////////////////////////////////////
#ifndef __AIEFRAMEWORK_H_
#define __AIEFRAMEWORK_H_
//////////////////////////////////////////////////////////////////////////
#include <vector>
#include "MathHelper.h"

namespace AIE
{

//////////////////////////////////////////////////////////////////////////
struct Vertex
{
	union
	{
		struct  
		{
			vec4 position;
			vec2 uv;
		};
		struct
		{
			float x, y, z, w;
			float u, v;
		};
	};

	bool operator == (const Vertex& rhs)
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w && u == rhs.u && v == rhs.v);
	}
};

//////////////////////////////////////////////////////////////////////////
//For arguments sake let's assume that we would like to have a vertex that 
//supports colours as well as UV's and vertex coordinates
//////////////////////////////////////////////////////////////////////////
struct VertexC
{
	union
	{
		struct  
		{
			vec4 position;
			vec2 uv;
			vec4 color;
		};
		struct
		{
			float x, y, z, w;
			float u, v;
			float r, g, b, a;
		};
	};

	bool operator == (const VertexC& rhs)
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w && u == rhs.u && v == rhs.v && r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a );
	}
};

// loads a texture of various formats
// optionally can fill in the width/height pointers if they are valid
unsigned int LoadTexture(const char* a_szTexture, unsigned int a_uiFormat = 0x1908 /*== GL_RGBA*/, unsigned int* a_uiWidth = nullptr, unsigned int* a_uiHeight = nullptr, unsigned int* a_uiBPP = nullptr );

// creates a shader program using the specified shader code files and a list of input attributes and output attributes
unsigned int LoadShader(unsigned int a_uiInputAttributeCount, const char** a_aszInputAttributes,
						unsigned int a_uiOutputAttributeCount, const char** a_aszOutputAttributes,
						const char* a_szVertexShader, const char* a_szPixelShader,
						const char* a_szGeometryShader = nullptr,
						const char* a_szTessellationControlShader = nullptr, const char* a_szTessellationEvaluationShader = nullptr);

// helper function for loading shader code into memory
char* FileToBuffer(const char* a_szPath);

// builds a textured plane out of 2 triangles and fills in the vertex array, vertex buffer, and index buffer
void Build3DPlane(float a_fSize, unsigned int& a_ruiVAO, unsigned int& a_ruiVBO, unsigned int& a_ruiIBO);

// utilities for timing; Get() updates timers and returns time since last Get call
void ResetTimer();
float GetDeltaTime();

// utility for mouse / keyboard movement of a matrix frame (suitable for camera)
void FreeMovement(float a_fDeltaTime, AIE::mat4& a_rmFrame, float a_fSpeed, const AIE::vec4& a_rvUp = AIE::vec4(0,1,0,0));

}//namespace AIE

//////////////////////////////////////////////////////////////////////////
#endif // __AIEFRAMEWORK_H_
//////////////////////////////////////////////////////////////////////////