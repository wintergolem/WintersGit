#ifndef _MESHNODE_H_
#define _MESHNODE_H_

#include "GL/glew.h"
#include <vector>
#include "MathHelper.h"
#include "SceneNode.h"
#include "Utilities.h"

class MeshNode : public SceneNode
{
public:
								MeshNode( AIE::vec4 a_translation, SceneNode *a_pParent = nullptr );
								~MeshNode();
	void						CreateBuffers();
	void						UpdateBuffers();
	GLuint						GetTexture()				{ return m_iTextureID; }
	GLuint						GetSecondaryTexture()		{ return m_iSecondaryTextureID; }
	GLuint						GetDisplacementTexture()	{ return m_iDisplacementTexID; }
	AIE::vec4					GetColor()					{ return m_vColour; }
	void						SetTexture( GLuint a_uiTextureID )			{ m_iTextureID = a_uiTextureID; }
	void						SetSecondaryTexture( GLuint a_uiTextureID ) { m_iSecondaryTextureID = a_uiTextureID; }
	void						SetDisplacementTexture(GLuint a_uiTextureID){ m_iDisplacementTexID = a_uiTextureID; }
	void						SetColor( AIE::vec4 a_vColour ){ m_vColour = a_vColour; }
	void						TranslateNode( AIE::vec4 a_vTrans );
	void						RotateNode( Quaternion &a_qRot );
	virtual void				Update( float a_fDeltaTime );
	void						Draw();

protected:
	GLuint						m_iVAO;
	GLuint						m_iVBO;
	GLuint						m_iIBO;

	GLuint						m_iTextureID;
	GLuint						m_iSecondaryTextureID;
	GLuint						m_iDisplacementTexID;
	GLuint						m_iShaderID;

	std::vector<AIE::Vertex>	m_aoVertices;
	std::vector<unsigned int>	m_auiIndex;

	unsigned int				m_iNumVerts;
	unsigned int				m_iNumIndices;

	AIE::vec4					m_vColour;

};

#endif