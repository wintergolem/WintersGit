#include "MeshNode.h"


using glm::vec4;

void MeshNode::MeshNode(vec4 a_translation, SceneNode *a_parent = nullptr )
{

}

void MeshNode::CreateBuffers()
{

}

void MeshNode::UpdateBuffers()
{

}

GLuint MeshNode::GetTexture( GLuint a_uiTextureID)
{

}

GLuint MeshNode::GetSecondaryTexture(GLuint a_uiTextureID)
{

}

GLuint MeshNode::GetDisplacementTexture()
{

}

vec4 MeshNode::GetColor()
{

}

void MeshNode::SetTexture( GLuint a_uiTextureID )
{

}
void MeshNode::SetSecondaryTexture( GLuint a_uiTextureID )
{

}

void MeshNode::SetDisplacementTexture(GLuint a_uiTextureID)
{

}

void MeshNode::SetColor( vec4 a_vColor)
{

}

void MeshNode::TranslateNode(vec4 a_vTrans)
{

}

void MeshNode::RotateNode(vec4 a_qRot)
{

}

virtual void MeshNode::Update(float a_fDeltaTime)
{

}

void MeshNode::Draw()
{

}