//////////////////////////////////////////////////////////////////////////
// Author:	Conan Bourke
// Date:	December 4 2012
// Brief:	A helper class for visualising lines and shapes
//////////////////////////////////////////////////////////////////////////
#include "Visualiser.h"
#include <GL/glew.h>

namespace AIE
{

Visualiser* Visualiser::sm_pSingleton = nullptr;

//////////////////////////////////////////////////////////////////////////
Visualiser::Visualiser(unsigned int a_uiMaxLines, unsigned int a_uiMaxTris)
	: m_uiMaxLines(a_uiMaxLines),
	m_uiMaxTris(a_uiMaxTris),
	m_uiLineCount(0),
	m_uiTriCount(0),
	m_uiATriCount(0),
	m_aoLines(new VisualiserLine[a_uiMaxLines]),
	m_aoTris(new VisualiserTri[a_uiMaxTris]),
	m_aoATris(new VisualiserTri[a_uiMaxTris])
{
	// create shaders
	char* vsSource = "#version 330\n\
					 in vec4 Position; \
					 in vec4 Colour; \
					 out vec4 vColour; \
					 uniform mat4 Projection; \
					 uniform mat4 View; \
					 void main() { vColour = Colour; gl_Position = Projection * View * Position; }";

	char* fsSource = "#version 330\n \
					 in vec4 vColour; \
					 out vec4 outColour; \
					 void main()	{ outColour = vColour; }";

	GLuint vsHandle = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsHandle = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vsHandle, 1, (const char**)&vsSource, 0);
	glCompileShader(vsHandle);

	glShaderSource(fsHandle, 1, (const char**)&fsSource, 0);
	glCompileShader(fsHandle);

	m_ShaderID = glCreateProgram();
	glAttachShader(m_ShaderID, vsHandle);
	glAttachShader(m_ShaderID, fsHandle);
	glBindAttribLocation(m_ShaderID, 0, "Position");
	glBindAttribLocation(m_ShaderID, 1, "Colour");
	glBindFragDataLocation(m_ShaderID, 0, "outColour");
	glLinkProgram(m_ShaderID);
	
	// create VBOs
	glGenBuffers( 1, &m_LineVBO );
	glBindBuffer(GL_ARRAY_BUFFER, m_LineVBO);
	glBufferData(GL_ARRAY_BUFFER, m_uiMaxLines * sizeof(VisualiserLine), m_aoLines, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers( 1, &m_TriVBO );
	glBindBuffer(GL_ARRAY_BUFFER, m_TriVBO);
	glBufferData(GL_ARRAY_BUFFER, m_uiMaxTris * sizeof(VisualiserTri), m_aoTris, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers( 1, &m_ATriVBO );
	glBindBuffer(GL_ARRAY_BUFFER, m_ATriVBO);
	glBufferData(GL_ARRAY_BUFFER, m_uiMaxTris * sizeof(VisualiserTri), m_aoATris, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int uiIndexCount = m_uiMaxTris * 3;
	unsigned int* auiIndices = new unsigned int[uiIndexCount];
	for ( unsigned int i = 0 ; i < uiIndexCount ; ++i )
		auiIndices[i] = i;
	glGenBuffers( 1, &m_TriIBO );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_TriIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, uiIndexCount * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers( 1, &m_ATriIBO );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ATriIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, uiIndexCount * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);
	delete[] auiIndices;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	uiIndexCount = m_uiMaxLines * 2;
	auiIndices = new unsigned int[uiIndexCount];
	for ( unsigned int i = 0 ; i < uiIndexCount ; ++i )
		auiIndices[i] = i;
	glGenBuffers( 1, &m_LineIBO );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_LineIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, uiIndexCount * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);
	delete[] auiIndices;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &m_LineVAO);
	glBindVertexArray(m_LineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_LineVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_LineIBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VisualiserVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(VisualiserVertex), ((char*)0) + 16);

	glGenVertexArrays(1, &m_TriVAO);
	glBindVertexArray(m_TriVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_TriVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_TriIBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VisualiserVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(VisualiserVertex), ((char*)0) + 16);

	glGenVertexArrays(1, &m_ATriVAO);
	glBindVertexArray(m_ATriVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ATriVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ATriIBO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VisualiserVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(VisualiserVertex), ((char*)0) + 16);
}

//////////////////////////////////////////////////////////////////////////
Visualiser::~Visualiser()
{
	delete[] m_aoLines;
	delete[] m_aoTris;
	delete[] m_aoATris;
	glDeleteBuffers( 1, &m_LineVBO );
	glDeleteBuffers( 1, &m_LineIBO );
	glDeleteBuffers( 1, &m_TriVBO );
	glDeleteBuffers( 1, &m_TriIBO );
	glDeleteBuffers( 1, &m_ATriVBO );
	glDeleteBuffers( 1, &m_ATriIBO );
	glDeleteVertexArrays( 1, &m_LineVAO );
	glDeleteVertexArrays( 1, &m_TriVAO );
	glDeleteVertexArrays( 1, &m_ATriVAO );
	glDeleteShader(m_ShaderID);
}

//////////////////////////////////////////////////////////////////////////
Visualiser* Visualiser::Create(unsigned int a_uiMaxLines /* = 16384 */, unsigned int a_uiMaxTris /* = 16384 */)
{
	if (sm_pSingleton == nullptr)
		sm_pSingleton = new Visualiser(a_uiMaxLines,a_uiMaxTris);
	return sm_pSingleton;
}

//////////////////////////////////////////////////////////////////////////
void Visualiser::Destroy()
{
	delete sm_pSingleton;
	sm_pSingleton = nullptr;
}

//////////////////////////////////////////////////////////////////////////
void Visualiser::Clear()
{
	m_uiLineCount = 0;
	m_uiTriCount = 0;
	m_uiATriCount = 0;
}

// Adds 3 unit-length lines (red,green,blue) representing the 3 axis of a transform, 
// at the transform's translation. Optional scale available.
void Visualiser::AddTransform(const mat4& a_rmTransform, float a_fScale /* = 1.0f */)
{
	vec4 vTranslate = a_rmTransform.row3;
	vec4 vXAxis = vTranslate + a_rmTransform.row0 * a_fScale;
	vec4 vYAxis = vTranslate + a_rmTransform.row1 * a_fScale;
	vec4 vZAxis = vTranslate + a_rmTransform.row2 * a_fScale;

	vec4 vRed(1,0,0,1);
	vec4 vGreen(0,1,0,1);
	vec4 vBlue(0,0,1,1);

	AddLine(vTranslate, vXAxis, vRed, vRed);
	AddLine(vTranslate, vYAxis, vGreen, vGreen);
	AddLine(vTranslate, vZAxis, vBlue, vBlue);
}

//////////////////////////////////////////////////////////////////////////
void Visualiser::AddAABB(const vec4& a_rvCenter, 
	const vec4& a_rvExtents, 
	const vec4& a_rvColour, 
	const mat4* a_pTransform /* = 0 */)
{
	vec4 vVerts[8];
	vec4 vX(a_rvExtents.x, 0, 0, 0);
	vec4 vY(0, a_rvExtents.y, 0, 0);
	vec4 vZ(0, 0, a_rvExtents.z, 0);

	if (a_pTransform)
	{
		mat4 t = a_pTransform->Transpose();
		vX = t * vX;
		vY = t * vY;
		vZ = t * vZ;
	}

	// top verts
	vVerts[0] = a_rvCenter - vX - vZ - vY;
	vVerts[1] = a_rvCenter - vX + vZ - vY;
	vVerts[2] = a_rvCenter + vX + vZ - vY;
	vVerts[3] = a_rvCenter + vX - vZ - vY;

	// bottom verts
	vVerts[4] = a_rvCenter - vX - vZ + vY;
	vVerts[5] = a_rvCenter - vX + vZ + vY;
	vVerts[6] = a_rvCenter + vX + vZ + vY;
	vVerts[7] = a_rvCenter + vX - vZ + vY;

	AddLine(vVerts[0], vVerts[1], a_rvColour, a_rvColour);
	AddLine(vVerts[1], vVerts[2], a_rvColour, a_rvColour);
	AddLine(vVerts[2], vVerts[3], a_rvColour, a_rvColour);
	AddLine(vVerts[3], vVerts[0], a_rvColour, a_rvColour);

	AddLine(vVerts[4], vVerts[5], a_rvColour, a_rvColour);
	AddLine(vVerts[5], vVerts[6], a_rvColour, a_rvColour);
	AddLine(vVerts[6], vVerts[7], a_rvColour, a_rvColour);
	AddLine(vVerts[7], vVerts[4], a_rvColour, a_rvColour);

	AddLine(vVerts[0], vVerts[4], a_rvColour, a_rvColour);
	AddLine(vVerts[1], vVerts[5], a_rvColour, a_rvColour);
	AddLine(vVerts[2], vVerts[6], a_rvColour, a_rvColour);
	AddLine(vVerts[3], vVerts[7], a_rvColour, a_rvColour);
}

//////////////////////////////////////////////////////////////////////////
void Visualiser::AddAABBFilled(const vec4& a_rvCenter, 
	const vec4& a_rvExtents, 
	const vec4& a_rvFillColour, 
	const mat4* a_pTransform /* = 0 */)
{
	vec4 vVerts[8];
	vec4 vX(a_rvExtents.x, 0, 0, 0);
	vec4 vY(0, a_rvExtents.y, 0, 0);
	vec4 vZ(0, 0, a_rvExtents.z, 0);

	if (a_pTransform)
	{
		mat4 t = a_pTransform->Transpose();
		vX = t * vX;
		vY = t * vY;
		vZ = t * vZ;
	}

	// top verts
	vVerts[0] = a_rvCenter - vX - vZ - vY;
	vVerts[1] = a_rvCenter - vX + vZ - vY;
	vVerts[2] = a_rvCenter + vX + vZ - vY;
	vVerts[3] = a_rvCenter + vX - vZ - vY;

	// bottom verts
	vVerts[4] = a_rvCenter - vX - vZ + vY;
	vVerts[5] = a_rvCenter - vX + vZ + vY;
	vVerts[6] = a_rvCenter + vX + vZ + vY;
	vVerts[7] = a_rvCenter + vX - vZ + vY;

	vec4 vWhite(1,1,1,1);

	AddLine(vVerts[0], vVerts[1], vWhite, vWhite);
	AddLine(vVerts[1], vVerts[2], vWhite, vWhite);
	AddLine(vVerts[2], vVerts[3], vWhite, vWhite);
	AddLine(vVerts[3], vVerts[0], vWhite, vWhite);

	AddLine(vVerts[4], vVerts[5], vWhite, vWhite);
	AddLine(vVerts[5], vVerts[6], vWhite, vWhite);
	AddLine(vVerts[6], vVerts[7], vWhite, vWhite);
	AddLine(vVerts[7], vVerts[4], vWhite, vWhite);

	AddLine(vVerts[0], vVerts[4], vWhite, vWhite);
	AddLine(vVerts[1], vVerts[5], vWhite, vWhite);
	AddLine(vVerts[2], vVerts[6], vWhite, vWhite);
	AddLine(vVerts[3], vVerts[7], vWhite, vWhite);

	// top
	AddTri(vVerts[0], vVerts[1], vVerts[2], a_rvFillColour);
	AddTri(vVerts[0], vVerts[2], vVerts[3], a_rvFillColour);

	// bottom
	AddTri(vVerts[4], vVerts[6], vVerts[5], a_rvFillColour);
	AddTri(vVerts[4], vVerts[7], vVerts[6], a_rvFillColour);

	// front
	AddTri(vVerts[0], vVerts[3], vVerts[4], a_rvFillColour);
	AddTri(vVerts[4], vVerts[3], vVerts[7], a_rvFillColour);

	// back
	AddTri(vVerts[5], vVerts[2], vVerts[1], a_rvFillColour);
	AddTri(vVerts[5], vVerts[6], vVerts[2], a_rvFillColour);

	// left
	AddTri(vVerts[4], vVerts[1], vVerts[0], a_rvFillColour);
	AddTri(vVerts[4], vVerts[5], vVerts[1], a_rvFillColour);

	// right
	AddTri(vVerts[7], vVerts[3], vVerts[2], a_rvFillColour);
	AddTri(vVerts[7], vVerts[2], vVerts[6], a_rvFillColour);
}


//////////////////////////////////////////////////////////////////////////
void Visualiser::AddCylinderFilled(const vec4& a_rvCenter, float a_fRadius, float a_fHalfLength,
	unsigned int a_uiSegments, const vec4& a_rvFillColour, const mat4* a_pmTransform /* = 0 */)
{
	vec4 vWhite(1,1,1,1);

	float fSegmentSize = (2 * PI) / a_uiSegments;

	mat4 t(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	if (a_pmTransform)
	{
		t = a_pmTransform->Transpose();
	}

	for ( unsigned int i = 0 ; i < a_uiSegments ; ++i )
	{
		vec4 v0top(0,a_fHalfLength,0,0);
		vec4 v1top( sinf( i * fSegmentSize ) * a_fRadius, a_fHalfLength, cosf( i * fSegmentSize ) * a_fRadius,0 );
		vec4 v2top( sinf( (i+1) * fSegmentSize ) * a_fRadius, a_fHalfLength, cosf( (i+1) * fSegmentSize ) * a_fRadius,0 );
		vec4 v0bottom(0,-a_fHalfLength,0,0);
		vec4 v1bottom( sinf( i * fSegmentSize ) * a_fRadius, -a_fHalfLength, cosf( i * fSegmentSize ) * a_fRadius,0 );
		vec4 v2bottom( sinf( (i+1) * fSegmentSize ) * a_fRadius, -a_fHalfLength, cosf( (i+1) * fSegmentSize ) * a_fRadius ,0);

		v0top = t * v0top;
		v1top = t * v1top;
		v2top = t * v2top;
		v0bottom = t * v0bottom;
		v1bottom = t * v1bottom;
		v2bottom = t * v2bottom;

		// triangles
		AddTri( a_rvCenter + v2top, a_rvCenter + v1top, a_rvCenter + v0top, a_rvFillColour);
		AddTri( a_rvCenter + v1bottom, a_rvCenter + v2bottom, a_rvCenter + v0bottom, a_rvFillColour);
		AddTri( a_rvCenter + v1bottom, a_rvCenter + v1top, a_rvCenter + v2top, a_rvFillColour);
		AddTri( a_rvCenter + v2top, a_rvCenter + v2bottom, a_rvCenter + v1bottom, a_rvFillColour);

		// lines
		AddLine(a_rvCenter + v1top, a_rvCenter + v2top, vWhite, vWhite);
		AddLine(a_rvCenter + v1top, a_rvCenter + v1bottom, vWhite, vWhite);
		AddLine(a_rvCenter + v1bottom, a_rvCenter + v2bottom, vWhite, vWhite);
	}
}

//////////////////////////////////////////////////////////////////////////
void Visualiser::AddRing(const vec4& a_rvCenter, float a_fInnerRadius, float a_fOuterRadius,
	unsigned int a_uiSegments, const vec4& a_rvFillColour, const mat4* a_pmTransform /* = 0 */)
{
	vec4 vSolid = a_rvFillColour;
	vSolid.w = 1;

	float fSegmentSize = (2 * PI) / a_uiSegments;

	mat4 t(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	if (a_pmTransform)
	{
		t = a_pmTransform->Transpose();
	}

	for ( unsigned int i = 0 ; i < a_uiSegments ; ++i )
	{
		vec4 v1outer( sinf( i * fSegmentSize ) * a_fOuterRadius, 0, cosf( i * fSegmentSize ) * a_fOuterRadius, 0 );
		vec4 v2outer( sinf( (i+1) * fSegmentSize ) * a_fOuterRadius, 0, cosf( (i+1) * fSegmentSize ) * a_fOuterRadius, 0 );
		vec4 v1inner( sinf( i * fSegmentSize ) * a_fInnerRadius, 0, cosf( i * fSegmentSize ) * a_fInnerRadius, 0 );
		vec4 v2inner( sinf( (i+1) * fSegmentSize ) * a_fInnerRadius, 0, cosf( (i+1) * fSegmentSize ) * a_fInnerRadius, 0 );

		if (a_pmTransform)
		{
			v1outer = t * v1outer;
			v2outer = t * v2outer;
			v1inner = t * v1inner;
			v2inner = t * v2inner;
		}

		if (a_rvFillColour.w != 0)
		{
			AddTri(a_rvCenter + v2outer, a_rvCenter + v1outer, a_rvCenter + v1inner, a_rvFillColour);
			AddTri(a_rvCenter + v1inner, a_rvCenter + v2inner, a_rvCenter + v2outer, a_rvFillColour);

			AddTri(a_rvCenter + v1inner, a_rvCenter + v1outer, a_rvCenter + v2outer, a_rvFillColour);
			AddTri(a_rvCenter + v2outer, a_rvCenter + v2inner, a_rvCenter + v1inner, a_rvFillColour);
		}
		else
		{
			// line
			AddLine(a_rvCenter + v1inner + a_rvCenter, a_rvCenter + v2inner + a_rvCenter, vSolid, vSolid);
			AddLine(a_rvCenter + v1outer + a_rvCenter, a_rvCenter + v2outer + a_rvCenter, vSolid, vSolid);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void Visualiser::AddDisk(const vec4& a_rvCenter, float a_fRadius,
	unsigned int a_uiSegments, const vec4& a_rvFillColour, const mat4* a_pmTransform /* = 0 */)
{
	vec4 vSolid = a_rvFillColour;
	vSolid.w = 1;

	float fSegmentSize = (2 * PI) / a_uiSegments;

	mat4 t(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	if (a_pmTransform)
	{
		t = a_pmTransform->Transpose();
	}

	for ( unsigned int i = 0 ; i < a_uiSegments ; ++i )
	{
		vec4 v1outer( sinf( i * fSegmentSize ) * a_fRadius, 0, cosf( i * fSegmentSize ) * a_fRadius, 0 );
		vec4 v2outer( sinf( (i+1) * fSegmentSize ) * a_fRadius, 0, cosf( (i+1) * fSegmentSize ) * a_fRadius, 0 );

		if (a_pmTransform)
		{
			v1outer = t * v1outer;
			v2outer = t * v2outer;
		}

		if (a_rvFillColour.w != 0)
		{
			AddTri(a_rvCenter, v1outer, a_rvCenter + v2outer, a_rvFillColour);
			AddTri(a_rvCenter + v2outer, a_rvCenter + v1outer, a_rvCenter, a_rvFillColour);
		}
		else
		{
			// line
			AddLine(a_rvCenter + v1outer, a_rvCenter + v2outer, vSolid, vSolid);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void Visualiser::AddArc(const vec4& a_rvCenter, float a_fRotation,
	float a_fRadius, float a_fArcHalfAngle,
	unsigned int a_uiSegments, const vec4& a_rvFillColour, const mat4* a_pmTransform /* = 0 */)
{
	vec4 vSolid = a_rvFillColour;
	vSolid.w = 1;

	float fSegmentSize = (2 * a_fArcHalfAngle) / a_uiSegments;

	mat4 t(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	if (a_pmTransform)
	{
		t = a_pmTransform->Transpose();
	}

	for ( unsigned int i = 0 ; i < a_uiSegments ; ++i )
	{
		vec4 v1outer( sinf( i * fSegmentSize - a_fArcHalfAngle + a_fRotation ) * a_fRadius, 0, cosf( i * fSegmentSize - a_fArcHalfAngle + a_fRotation ) * a_fRadius, 0 );
		vec4 v2outer( sinf( (i+1) * fSegmentSize - a_fArcHalfAngle + a_fRotation ) * a_fRadius, 0, cosf( (i+1) * fSegmentSize - a_fArcHalfAngle + a_fRotation ) * a_fRadius, 0 );

		if (a_pmTransform)
		{
			v1outer = t * v1outer;
			v2outer = t * v2outer;
		}

		if (a_rvFillColour.w != 0)
		{
			AddTri(a_rvCenter, a_rvCenter + v1outer, a_rvCenter + v2outer, a_rvFillColour);
			AddTri(a_rvCenter + v2outer, a_rvCenter + v1outer, a_rvCenter, a_rvFillColour);
		}
		else
		{
			// line
			AddLine(a_rvCenter + v1outer, a_rvCenter + v2outer, vSolid, vSolid);
		}
	}

	// edge lines
	if (a_rvFillColour.w == 0)
	{
		vec4 v1outer( sinf( -a_fArcHalfAngle + a_fRotation ) * a_fRadius, 0, cosf( -a_fArcHalfAngle + a_fRotation ) * a_fRadius, 0 );
		vec4 v2outer( sinf( a_fArcHalfAngle + a_fRotation ) * a_fRadius, 0, cosf( a_fArcHalfAngle + a_fRotation ) * a_fRadius, 0 );

		if (a_pmTransform)
		{
			v1outer = t * v1outer;
			v2outer = t * v2outer;
		}

		AddLine(a_rvCenter, a_rvCenter + v1outer, vSolid, vSolid);
		AddLine(a_rvCenter, a_rvCenter + v2outer, vSolid, vSolid);
	}
}

//////////////////////////////////////////////////////////////////////////
void Visualiser::AddArcRing(const vec4& a_rvCenter, float a_fRotation, 
	float a_fInnerRadius, float a_fOuterRadius, float a_fArcHalfAngle,
	unsigned int a_uiSegments, const vec4& a_rvFillColour, const mat4* a_pmTransform /* = 0 */)
{
	vec4 vSolid = a_rvFillColour;
	vSolid.w = 1;

	float fSegmentSize = (2 * a_fArcHalfAngle) / a_uiSegments;

	mat4 t(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	if (a_pmTransform)
	{
		t = a_pmTransform->Transpose();
	}

	for ( unsigned int i = 0 ; i < a_uiSegments ; ++i )
	{
		vec4 v1outer( sinf( i * fSegmentSize - a_fArcHalfAngle + a_fRotation ) * a_fOuterRadius, 0, cosf( i * fSegmentSize - a_fArcHalfAngle + a_fRotation ) * a_fOuterRadius, 0 );
		vec4 v2outer( sinf( (i+1) * fSegmentSize - a_fArcHalfAngle + a_fRotation ) * a_fOuterRadius, 0, cosf( (i+1) * fSegmentSize - a_fArcHalfAngle + a_fRotation ) * a_fOuterRadius, 0 );
		vec4 v1inner( sinf( i * fSegmentSize - a_fArcHalfAngle + a_fRotation  ) * a_fInnerRadius, 0, cosf( i * fSegmentSize - a_fArcHalfAngle + a_fRotation  ) * a_fInnerRadius, 0 );
		vec4 v2inner( sinf( (i+1) * fSegmentSize - a_fArcHalfAngle + a_fRotation  ) * a_fInnerRadius, 0, cosf( (i+1) * fSegmentSize - a_fArcHalfAngle + a_fRotation  ) * a_fInnerRadius, 0 );

		if (a_pmTransform)
		{
			v1outer = t * v1outer;
			v2outer = t * v2outer;
			v1inner = t * v1inner;
			v2inner = t * v2inner;
		}

		if (a_rvFillColour.w != 0)
		{
			AddTri(a_rvCenter + v1inner, a_rvCenter + v1outer, a_rvCenter + v2outer, a_rvFillColour);
			AddTri(a_rvCenter + v2outer, a_rvCenter + v2inner, a_rvCenter + v1inner, a_rvFillColour);

			AddTri(a_rvCenter + v2outer, a_rvCenter + v1outer, a_rvCenter + v1inner, a_rvFillColour);
			AddTri(a_rvCenter + v1inner, a_rvCenter + v2inner, a_rvCenter + v2outer, a_rvFillColour);
		}
		else
		{
			// line
			AddLine(a_rvCenter + v1inner, a_rvCenter + v2inner, vSolid, vSolid);
			AddLine(a_rvCenter + v1outer, a_rvCenter + v2outer, vSolid, vSolid);
		}
	}

	// edge lines
	if (a_rvFillColour.w == 0)
	{
		vec4 v1outer( sinf( -a_fArcHalfAngle + a_fRotation ) * a_fOuterRadius, 0, cosf( -a_fArcHalfAngle + a_fRotation ) * a_fOuterRadius, 0 );
		vec4 v2outer( sinf( a_fArcHalfAngle + a_fRotation ) * a_fOuterRadius, 0, cosf( a_fArcHalfAngle + a_fRotation ) * a_fOuterRadius, 0 );
		vec4 v1inner( sinf( -a_fArcHalfAngle + a_fRotation  ) * a_fInnerRadius, 0, cosf( -a_fArcHalfAngle + a_fRotation  ) * a_fInnerRadius, 0 );
		vec4 v2inner( sinf( a_fArcHalfAngle + a_fRotation  ) * a_fInnerRadius, 0, cosf( a_fArcHalfAngle + a_fRotation  ) * a_fInnerRadius, 0 );

		if (a_pmTransform)
		{
			v1outer = t * v1outer;
			v2outer = t * v2outer;
			v1inner = t * v1inner;
			v2inner = t * v2inner;
		}

		AddLine(a_rvCenter + v1inner, a_rvCenter + v1outer, vSolid, vSolid);
		AddLine(a_rvCenter + v2inner, a_rvCenter + v2outer, vSolid, vSolid);
	}
}

///////////////////////////////////////////////////////////////////////////
void Visualiser::AddSphere(const vec4& a_vCenter, int numRows, int numColumns, float radius, const vec4& a_rvFillColour, 
								const mat4* a_pmTransform /*= nullptr*/, float longMin /*= 0.f*/, float longMax /*= 360*/, 
								float latMin /*= -90*/, float latMax /*= 90*/)
{
	mat4 t(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	if (a_pmTransform)
	{
		t = a_pmTransform->Transpose();
	}

	float inverseRadius = 1/radius;
	//Invert these first as the multiply is slightly quicker
	float invColumns = 1.0f/float(numColumns);
	float invRows = 1.0f/float(numRows);
	
	//Lets put everything in radians first
	float latitiudinalRange = (latMax-latMin) * DEG2RAD;
	float longitudinalRange = (longMax-longMin) * DEG2RAD;
	// for each row of the mesh
	vec4* v4Array = new vec4[numRows*numColumns + numColumns];

	for (int row = 0; row <= numRows; ++row)
	{
		// y ordinates this may be a little confusing but here we are navigating around the xAxis in GL
		float ratioAroundXAxis = float(row) * invRows;
		float u_textureCoordinate = fabsf(1.0f-ratioAroundXAxis);
		float radiansAboutXAxis  = ratioAroundXAxis * latitiudinalRange + (latMin * DEG2RAD);
		float y  =  radius * sin(radiansAboutXAxis);
		float z  =  radius * cos(radiansAboutXAxis);
		
		for ( int col = 0; col <= numColumns; ++col )
		{
			float ratioAroundYAxis   = float(col) * invColumns;
			float v_textureCoordinate = fabsf(ratioAroundYAxis - 1.0f);
			float theta = ratioAroundYAxis * longitudinalRange + (longMin * DEG2RAD);
			vec4 v4Point = t * vec4( -z * sinf(theta), y, -z * cosf(theta), 0 );
			vec4 v4Normal = t * vec4( inverseRadius * v4Point.x, inverseRadius * v4Point.y, inverseRadius * v4Point.z, 0.f);
			vec2 v2TextureCoords = vec2( u_textureCoordinate, v_textureCoordinate);
			int index = row * numColumns + (col % numColumns);
			v4Array[index] = v4Point;
		}
	}
	
	for (int face = 0; face < (numRows)*(numColumns); ++face )
	{
		int iNextFace = face+1;
		
		
		if( iNextFace % numColumns == 0 )
		{
			iNextFace = iNextFace - (numColumns);
		}

		AddLine(a_vCenter + v4Array[face], a_vCenter + v4Array[face+numColumns], vec4(1.f,1.f,1.f,1.f), vec4(1.f,1.f,1.f,1.f));
		
		if( face % numColumns == 0 && longitudinalRange < TWOPI)
		{
				continue;
		}
		AddLine(a_vCenter + v4Array[iNextFace+numColumns], a_vCenter + v4Array[face+numColumns], vec4(1.f,1.f,1.f,1.f), vec4(1.f,1.f,1.f,1.f));

		AddTri( a_vCenter + v4Array[iNextFace], a_vCenter + v4Array[face], a_vCenter + v4Array[iNextFace+numColumns], a_rvFillColour);
		AddTri( a_vCenter + v4Array[face], a_vCenter + v4Array[face+numColumns], a_vCenter + v4Array[iNextFace+numColumns], a_rvFillColour);		
	}

	delete[] v4Array;	
}

//////////////////////////////////////////////////////////////////////////
void Visualiser::AddHermiteSpline(const vec4& a_rvStart, const vec4& a_rvEnd,
	const vec4& a_rvTangentStart, const vec4& a_rvTangentEnd, unsigned int a_uiSegment, const vec4& a_rvColour)
{
	a_uiSegment = a_uiSegment > 1 ? a_uiSegment : 1;

	vec4 vPrev = a_rvStart;

	for ( unsigned int i = 1 ; i <= a_uiSegment ; ++i )
	{
		float s = i / (float)a_uiSegment;

		float s2 = s * s;
		float s3 = s2 * s;
		float h1 = (2.0f * s3) - (3.0f * s2) + 1.0f;
		float h2 = (-2.0f * s3) + (3.0f * s2);
		float h3 =  s3- (2.0f * s2) + s;
		float h4 = s3 - s2;
		vec4 p = (a_rvStart * h1) + (a_rvEnd * h2) + (a_rvTangentStart * h3) + (a_rvTangentEnd * h4);
		p.w = 1;

		AddLine(vPrev,p,a_rvColour,a_rvColour);
		vPrev = p;
	}
}

//////////////////////////////////////////////////////////////////////////
void Visualiser::AddLine(const vec4& a_rv0,  const vec4& a_rv1, 
	const vec4& a_rvColour)
{
	AddLine(a_rv0,a_rv1,a_rvColour,a_rvColour);
}

//////////////////////////////////////////////////////////////////////////
void Visualiser::AddLine(const vec4& a_rv0, const vec4& a_rv1, 
	const vec4& a_rvColour0, const vec4& a_rvColour1)
{
	if (m_uiLineCount < m_uiMaxLines)
	{
		m_aoLines[ m_uiLineCount ].v0.position = a_rv0;
		m_aoLines[ m_uiLineCount ].v0.colour = a_rvColour0;
		m_aoLines[ m_uiLineCount ].v1.position = a_rv1;
		m_aoLines[ m_uiLineCount ].v1.colour = a_rvColour1;

		++m_uiLineCount;
	}
}

void Visualiser::AddCustBoxWire(const vec4& a_rvCenter, 
	const vec4& a_rvExtents, 
	const vec4& a_rvColour, 
	const mat4* a_pTransform /* = 0 */)
{
	vec4 vVerts[9];
	vec4 vX(a_rvExtents.x, 0, 0, 0);
	vec4 vY(0, a_rvExtents.y, 0, 0);
	vec4 vZ(0, 0, a_rvExtents.z, 0);

	if (a_pTransform)
	{
		mat4 t = a_pTransform->Transpose();
		vX = t * vX;
		vY = t * vY;
		vZ = t * vZ;
	}

	// top verts
	vVerts[0] = a_rvCenter - vX - vZ - vY;
	vVerts[1] = a_rvCenter - vX + vZ - vY;
	vVerts[2] = a_rvCenter + vX + vZ - vY;
	vVerts[3] = a_rvCenter + vX - vZ - vY;

	// bottom verts
	vVerts[4] = a_rvCenter - vX - vZ + vY;
	vVerts[5] = a_rvCenter - vX + vZ + vY;
	vVerts[6] = a_rvCenter + vX + vZ + vY;
	vVerts[7] = a_rvCenter + vX - vZ + vY;

	//triangle on top vert
	vVerts[8] = a_rvCenter + (vX*0) +(vZ*0)+vY*2;
		//drawing cude
	AddLine(vVerts[0], vVerts[1], a_rvColour, a_rvColour);
	AddLine(vVerts[1], vVerts[2], a_rvColour, a_rvColour);
	AddLine(vVerts[2], vVerts[3], a_rvColour, a_rvColour);
	AddLine(vVerts[3], vVerts[0], a_rvColour, a_rvColour);

	AddLine(vVerts[4], vVerts[5], a_rvColour, a_rvColour);
	AddLine(vVerts[5], vVerts[6], a_rvColour, a_rvColour);
	AddLine(vVerts[6], vVerts[7], a_rvColour, a_rvColour);
	AddLine(vVerts[7], vVerts[4], a_rvColour, a_rvColour);

	AddLine(vVerts[0], vVerts[4], a_rvColour, a_rvColour);
	AddLine(vVerts[1], vVerts[5], a_rvColour, a_rvColour);
	AddLine(vVerts[2], vVerts[6], a_rvColour, a_rvColour);
	AddLine(vVerts[3], vVerts[7], a_rvColour, a_rvColour);

	//drawing prymid
	AddLine(vVerts[4], vVerts[8], a_rvColour, a_rvColour);
	AddLine(vVerts[5], vVerts[8], a_rvColour, a_rvColour);
	AddLine(vVerts[6], vVerts[8], a_rvColour, a_rvColour);
	AddLine(vVerts[7], vVerts[8], a_rvColour, a_rvColour);
}

void Visualiser::AddCustBoxFilled(const vec4& a_rvCenter, 
	const vec4& a_rvExtents, 
	const vec4& a_rvFillColour, 
	const mat4* a_pTransform /* = 0 */)
{
	vec4 vVerts[9];
	vec4 vX(a_rvExtents.x, 0, 0, 0);
	vec4 vY(0, a_rvExtents.y, 0, 0);
	vec4 vZ(0, 0, a_rvExtents.z, 0);

	if (a_pTransform)
	{
		mat4 t = a_pTransform->Transpose();
		vX = t * vX;
		vY = t * vY;
		vZ = t * vZ;
	}

	// top verts
	vVerts[0] = a_rvCenter - vX - vZ - vY;
	vVerts[1] = a_rvCenter - vX + vZ - vY;
	vVerts[2] = a_rvCenter + vX + vZ - vY;
	vVerts[3] = a_rvCenter + vX - vZ - vY;

	// bottom verts
	vVerts[4] = a_rvCenter - vX - vZ + vY;
	vVerts[5] = a_rvCenter - vX + vZ + vY;
	vVerts[6] = a_rvCenter + vX + vZ + vY;
	vVerts[7] = a_rvCenter + vX - vZ + vY;

	//p on top
	vVerts[8] = a_rvCenter + vY*2;

	vec4 vWhite(1,1,1,1);

	AddLine(vVerts[0], vVerts[1], vWhite, vWhite);
	AddLine(vVerts[1], vVerts[2], vWhite, vWhite);
	AddLine(vVerts[2], vVerts[3], vWhite, vWhite);
	AddLine(vVerts[3], vVerts[0], vWhite, vWhite);

	AddLine(vVerts[4], vVerts[5], vWhite, vWhite);
	AddLine(vVerts[5], vVerts[6], vWhite, vWhite);
	AddLine(vVerts[6], vVerts[7], vWhite, vWhite);
	AddLine(vVerts[7], vVerts[4], vWhite, vWhite);

	AddLine(vVerts[0], vVerts[4], vWhite, vWhite);
	AddLine(vVerts[1], vVerts[5], vWhite, vWhite);
	AddLine(vVerts[2], vVerts[6], vWhite, vWhite);
	AddLine(vVerts[3], vVerts[7], vWhite, vWhite);

	// top
	AddTri(vVerts[0], vVerts[1], vVerts[2], a_rvFillColour);
	AddTri(vVerts[0], vVerts[2], vVerts[3], a_rvFillColour);

	// bottom
	AddTri(vVerts[4], vVerts[6], vVerts[5], a_rvFillColour);
	AddTri(vVerts[4], vVerts[7], vVerts[6], a_rvFillColour);

	// front
	AddTri(vVerts[0], vVerts[3], vVerts[4], a_rvFillColour);
	AddTri(vVerts[4], vVerts[3], vVerts[7], a_rvFillColour);

	// back
	AddTri(vVerts[5], vVerts[2], vVerts[1], a_rvFillColour);
	AddTri(vVerts[5], vVerts[6], vVerts[2], a_rvFillColour);

	// left
	AddTri(vVerts[4], vVerts[1], vVerts[0], a_rvFillColour);
	AddTri(vVerts[4], vVerts[5], vVerts[1], a_rvFillColour);

	// right
	AddTri(vVerts[7], vVerts[3], vVerts[2], a_rvFillColour);
	AddTri(vVerts[7], vVerts[2], vVerts[6], a_rvFillColour);

	//Pyrmid
	AddTri(vVerts[1], vVerts[0], vVerts[8], vec4(1,0,0,1));
	AddTri(vVerts[2], vVerts[1], vVerts[8], vec4(1,0,0,1));
	AddTri(vVerts[3], vVerts[2], vVerts[8], vec4(1,0,0,1));
	AddTri(vVerts[3], vVerts[8], vVerts[0], vec4(1,0,0,1));
}
//////////////////////////////////////////////////////////////////////////
void Visualiser::AddTri(const vec4& a_rv0, const vec4& a_rv1, const vec4& a_rv2, const vec4& a_rvColour)
{
	if (a_rvColour.w == 1)
	{
		if (m_uiTriCount < m_uiMaxTris)
		{
			m_aoTris[ m_uiTriCount ].v0.position = a_rv0;
			m_aoTris[ m_uiTriCount ].v1.position = a_rv1;
			m_aoTris[ m_uiTriCount ].v2.position = a_rv2;
			m_aoTris[ m_uiTriCount ].v0.colour = a_rvColour;
			m_aoTris[ m_uiTriCount ].v1.colour = a_rvColour;
			m_aoTris[ m_uiTriCount ].v2.colour = a_rvColour;

			++m_uiTriCount;
		}
	}
	else
	{
		if (m_uiATriCount < m_uiMaxTris)
		{
			m_aoATris[ m_uiATriCount ].v0.position = a_rv0;
			m_aoATris[ m_uiATriCount ].v1.position = a_rv1;
			m_aoATris[ m_uiATriCount ].v2.position = a_rv2;
			m_aoATris[ m_uiATriCount ].v0.colour = a_rvColour;
			m_aoATris[ m_uiATriCount ].v1.colour = a_rvColour;
			m_aoATris[ m_uiATriCount ].v2.colour = a_rvColour;

			++m_uiATriCount;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void Visualiser::Draw(mat4* a_pmView, mat4* a_pmProjection)
{
	if (m_uiLineCount > 0 || m_uiTriCount > 0 || m_uiATriCount > 0)
	{
		glUseProgram(m_ShaderID);

		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GLuint ProjectionID = glGetUniformLocation(m_ShaderID,"Projection");
		GLuint ViewID = glGetUniformLocation(m_ShaderID,"View");

		glUniformMatrix4fv(ProjectionID, 1, false, (float*)a_pmProjection);
		glUniformMatrix4fv(ViewID, 1, false, (float*)a_pmView);

		if (m_uiLineCount > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_LineVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_uiLineCount * sizeof(VisualiserLine), m_aoLines);

			glBindVertexArray(m_LineVAO);
			glDrawElements(GL_LINES, m_uiLineCount * 2, GL_UNSIGNED_INT, 0);
		}

		if (m_uiTriCount > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_TriVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_uiTriCount * sizeof(VisualiserTri), m_aoTris);

			glBindVertexArray(m_TriVAO);
			glDrawElements(GL_TRIANGLES, m_uiTriCount * 3, GL_UNSIGNED_INT, 0);
		}

		if (m_uiATriCount > 0)
		{
			glDepthMask(false);
			glBindBuffer(GL_ARRAY_BUFFER, m_ATriVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_uiATriCount * sizeof(VisualiserTri), m_aoATris);

			glBindVertexArray(m_ATriVAO);
			glDrawElements(GL_TRIANGLES, m_uiATriCount * 3, GL_UNSIGNED_INT, 0);
			glDepthMask(true);
		}
	}
}

}//namespace AIE
