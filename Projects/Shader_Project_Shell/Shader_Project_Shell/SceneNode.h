#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include <list>
#include "MathHelper.h"
#include "Quaternion.h"
#include "Quaternion.h"

enum eNodeType
{	ABSTRACT	= 0,
	DRAWABLE	= 1,
	SWITCH		= 2	};

class SceneNode
{
public:
						SceneNode( AIE::vec4 a_translation, SceneNode *a_pParent = nullptr );
						~SceneNode();
	inline SceneNode 	*GetParent() { return m_pParent; }
	inline void 		SetParent( SceneNode *a_pParent ) { m_pParent = a_pParent; }
	virtual void		Update( float a_fDeltaTime );
	void				RegisterChild( SceneNode* a_child );
	void				DeregisterChild( SceneNode* a_child );
	void				DeleteAllChildren();
	AIE::mat4			GetWorldTransform();
	AIE::mat4&			GetLocalTransform() { return m_localTransform; }
	void				SetLocalTransform( AIE::mat4 a_transform )	{ m_localTransform = a_transform; }
	virtual void		TranslateNode( AIE::vec4 a_vTrans )			{ m_localTransform.row3 = a_vTrans; }
	//AIE::mat4&		GetRotation() { return m_rotation; }
	//void				SetRotation( AIE::mat4 a_rot ) { m_rotation = a_rot; }
	AIE::mat4			GetRotationMatrix();
	Quaternion&			GetRotation() { return m_qRotation; }
	void				SetRotation( Quaternion &a_qRot ) { m_qRotation = a_qRot; }
	virtual void		RotateNode( Quaternion &a_qRot );
	//vec4				GetParentTranslation();
	//mat4				GetParentRotation();

protected:
	eNodeType			m_eType;
	SceneNode 			*m_pParent;
	AIE::mat4			m_localTransform;
	Quaternion			m_qRotation;
	AIE::mat4			m_rotation;

	std::list<SceneNode*>	m_listOfChildren;
};

#endif