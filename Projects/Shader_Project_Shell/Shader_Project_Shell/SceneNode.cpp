#include "SceneNode.h"

SceneNode::SceneNode( AIE::vec4 a_translation, SceneNode *a_pParent )
{
	m_qRotation				= Quaternion( 1.f, 0.f, 0.f, 0.f );

	m_rotation				= AIE::mat4(	1.f, 0.f, 0.f, 0.f,
											0.f, 1.f, 0.f, 0.f,
											0.f, 0.f, 1.f, 0.f,
											0.f, 0.f, 0.f, 1.f	);
	m_localTransform		= AIE::mat4(	1.f, 0.f, 0.f, 0.f,
											0.f, 1.f, 0.f, 0.f,
											0.f, 0.f, 1.f, 0.f,
											0.f, 0.f, 0.f, 1.f	);

	m_localTransform.row3	= a_translation;
	m_pParent				= a_pParent;

	if( m_pParent != nullptr )
		m_pParent->RegisterChild( this );
}

SceneNode::~SceneNode()
{
	//if( m_pParent != nullptr )
		//m_pParent->DeregisterChild( this );
}

AIE::mat4 SceneNode::GetWorldTransform()
{
	if( m_pParent != nullptr )
		return m_localTransform * m_pParent->GetWorldTransform();
	return m_localTransform;
}

//void SceneNode::RotateNode( float a_fRad, const AIE::vec4& a_axis )
//{
//	m_rotation.RotateAxis( a_fRad, a_axis );
//}

AIE::mat4 SceneNode::GetRotationMatrix()
{
	return m_qRotation.ToMatrix();
}

void SceneNode::RotateNode( Quaternion &a_qRot )
{
	m_qRotation = a_qRot * m_qRotation;
}

void SceneNode::RegisterChild(SceneNode* a_child)
{
	m_listOfChildren.push_back( a_child );
}

void SceneNode::DeregisterChild(SceneNode* a_child)
{
	std::list<SceneNode*>::iterator iter;
	iter = m_listOfChildren.begin();

	while( iter != m_listOfChildren.end() )
	{
		if( *iter != a_child )
			continue;
		iter = m_listOfChildren.erase( iter );
		break;
	}
}

void SceneNode::DeleteAllChildren()
{
	auto iter = m_listOfChildren.begin();
	while( iter != m_listOfChildren.end() )
	{
		(*iter)->DeleteAllChildren();
		delete *iter;
		*iter = nullptr;
		iter = m_listOfChildren.erase( iter );
	}
}

void SceneNode::Update(float a_fDeltaTime)
{
	auto iter = m_listOfChildren.begin();
	while( iter != m_listOfChildren.end() )
	{
		(*iter)->Update( a_fDeltaTime );
		++iter;
	}
}

//vec4 SceneNode::GetParentTranslation()
//{
//	if( m_pParent != nullptr )
//		return m_pParent->GetWorldTransform().row3;
//	return m_localTransform.row3;
//}
//
//mat4 SceneNode::GetParentRotation()
//{
//	mat4 parentRotation;
//	if( m_pParent != nullptr )
//		parentRotation = m_pParent->GetWorldTransform();
//	else
//		parentRotation = m_localTransform;
//
//	parentRotation.row3 = vec4(0.f, 0.f, 0.f, 1.f);
//
//	return parentRotation;
//}