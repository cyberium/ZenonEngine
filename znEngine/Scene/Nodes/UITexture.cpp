#include "stdafx.h"

// General
#include "UITexture.h"

namespace
{
	const glm::vec4 cDefaultColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

CUITextureNode::CUITextureNode(IScene& Scene)
	: CUIControl(Scene)
{

}

CUITextureNode::~CUITextureNode()
{}


void CUITextureNode::Initialize()
{
	__super::Initialize();

	m_Material = MakeShared(UI_Texture_Material, GetRenderDevice());
	m_Material->SetColor(cDefaultColor);

	m_Size = glm::vec2(50.0f, 50.0f);

	m_Mesh = GetRenderDevice().GetObjectsFactory().CreateModel();
	m_Mesh->AddConnection(m_Material, GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(m_Size.x, m_Size.y));
}



//
// CUITextureNode
//
void CUITextureNode::SetTexture(std::shared_ptr<ITexture> _texture)
{
	m_Material->SetTexture(_texture);
}

void CUITextureNode::SetColor(glm::vec4 _color)
{
	m_Material->SetColor(_color);
}



//
// CUIControl
//
glm::vec2 CUITextureNode::GetSize() const
{
    return m_Size;
}

void CUITextureNode::AcceptMesh(IVisitor* visitor)
{
	m_Mesh->Accept(visitor);
}
