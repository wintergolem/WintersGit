//////////////////////////////////////////////////////////////////////////
// Author:	Conan Bourke
// Date:	December 4 2012
// Brief:	A helper class for visualising lines and shapes
//////////////////////////////////////////////////////////////////////////
#ifndef __VISUALISER_H_
#define __VISUALISER_H_
//////////////////////////////////////////////////////////////////////////

#include "MathHelper.h"

namespace AIE
{

// visualiser class for visualising lines / tris
class Visualiser
{
public:

	static Visualiser*	Create(unsigned int a_uiMaxLines = 16384, unsigned int a_uiMaxTris = 16384);
	static Visualiser*	Get()	{	return sm_pSingleton;	}
	static void			Destroy();

	// removes all visualisers
	void			Clear();

	// draws current visualiser buffers
	void			Draw(mat4* a_pmView, mat4* a_pmProjection);

	// Adds a single debug line
	void			AddLine(const vec4& a_rv0,  const vec4& a_rv1, 
							const vec4& a_rvColour);

	// Adds a single debug line
	void			AddLine(const vec4& a_rv0, const vec4& a_rv1, 
							const vec4& a_rvColour0, const vec4& a_rvColour1);

	// Adds a triangle.
	void			AddTri(const vec4& a_rv0, const vec4& a_rv1, const vec4& a_rv2, const vec4& a_rvColour);

	// Adds 3 unit-length lines (red,green,blue) representing the 3 axis of a transform, 
	// at the transform's translation. Optional scale available.
	void			AddTransform(const mat4& a_rmTransform, float a_fScale = 1.0f);
	
	// Adds a wireframe Axis-Aligned Bounding-Box with optional transform for rotation/translation.
	void			AddAABB(const vec4& a_rvCenter, const vec4& a_rvExtents, 
							const vec4& a_rvColour, const mat4* a_pTransform = nullptr);

	// Adds an Axis-Aligned Bounding-Box with optional transform for rotation.
	void			AddAABBFilled(const vec4& a_rvCenter, const vec4& a_rvExtents, 
								const vec4& a_rvFillColour, const mat4* a_pTransform = nullptr);

	// Adds a cylinder aligned to the Y-axis with optional transform for rotation.
	void			AddCylinderFilled(const vec4& a_rvCenter, float a_fRadius, float a_fHalfLength,
									unsigned int a_uiSegments, const vec4& a_rvFillColour, const mat4* a_pmTransform = nullptr);

	// Adds a double-sided hollow ring in the XZ axis with optional transform for rotation.
	// If a_rvFilLColour.w == 0 then only an outer and inner line is drawn.
	void			AddRing(const vec4& a_rvCenter, float a_fInnerRadius, float a_fOuterRadius,
							unsigned int a_uiSegments, const vec4& a_rvFillColour, const mat4* a_pmTransform = nullptr);

	// Adds a double-sided disk in the XZ axis with optional transform for rotation.
	// If a_rvFilLColour.w == 0 then only an outer line is drawn.
	void			AddDisk(const vec4& a_rvCenter, float a_fRadius,
							unsigned int a_uiSegments, const vec4& a_rvFillColour, const mat4* a_pmTransform = nullptr);

	// Adds an arc, around the Y-axis
	// If a_rvFilLColour.w == 0 then only an outer line is drawn.
	void			AddArc(const vec4& a_rvCenter, float a_fRotation,
							float a_fRadius, float a_fHalfAngle,
							unsigned int a_uiSegments, const vec4& a_rvFillColour, const mat4* a_pmTransform = nullptr);

	// Adds an arc, around the Y-axis, starting at the inner radius and extending to the outer radius
	// If a_rvFilLColour.w == 0 then only an outer line is drawn.
	void			AddArcRing(const vec4& a_rvCenter, float a_fRotation, 
								float a_fInnerRadius, float a_fOuterRadius, float a_fArcHalfAngle,
								unsigned int a_uiSegments, const vec4& a_rvFillColour, const mat4* a_pmTransform = nullptr);
	// Adds a Sphere at a given position, with a given number of rows, and columns, radius and a max and min long and latitude
	void			AddSphere(const vec4& a_vCenter, int numRows, int numColumns, float radius, const vec4& a_rvFillColour, 
								const mat4* a_pmTransform = nullptr, float longMin = 0.f, float longMax = 360, 
								float latMin = -90, float latMax = 90 );

	// Adds a single Hermite spline curve
	void			AddHermiteSpline(const vec4& a_rvStart, const vec4& a_rvEnd,
									const vec4& a_rvTangentStart, const vec4& a_rvTangentEnd, unsigned int a_uiSegments, const vec4& a_rvColour);

private:

	Visualiser(unsigned int a_uiMaxLines, unsigned int a_uiMaxTris);
	~Visualiser();

	struct VisualiserVertex
	{
		vec4 position;
		vec4 colour;
	};

	struct VisualiserLine
	{
		VisualiserVertex v0;
		VisualiserVertex v1;
	};

	struct VisualiserTri
	{
		VisualiserVertex v0;
		VisualiserVertex v1;
		VisualiserVertex v2;
	};

	unsigned int	m_ShaderID;

	// line data
	unsigned int	m_uiMaxLines;
	unsigned int	m_uiLineCount;
	VisualiserLine*	m_aoLines;

	unsigned int	m_LineVAO;
	unsigned int 	m_LineVBO;
	unsigned int 	m_LineIBO;

	// triangle data
	unsigned int	m_uiMaxTris;
	unsigned int	m_uiTriCount;
	VisualiserTri*	m_aoTris;

	unsigned int	m_TriVAO;
	unsigned int 	m_TriVBO;
	unsigned int 	m_TriIBO;

	// triangle data
	unsigned int	m_uiATriCount;
	VisualiserTri*	m_aoATris;

	unsigned int	m_ATriVAO;
	unsigned int 	m_ATriVBO;
	unsigned int 	m_ATriIBO;

	static Visualiser*	sm_pSingleton;
};

}//namespace AIE

//////////////////////////////////////////////////////////////////////////
#endif // __VISUALISER_H_
//////////////////////////////////////////////////////////////////////////
