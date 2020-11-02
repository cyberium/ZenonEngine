#include "stdafx.h"

// General
#include "UITexture.h"

namespace
{
	const glm::vec4 cDefaultColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

CUITextureNode::CUITextureNode(IRenderDevice& RenderDevice, glm::vec2 Size)
	: m_Size(Size)
{
	m_Material = MakeShared(UI_Texture_Material, RenderDevice);
	m_Material->SetColor(cDefaultColor);

	m_Mesh = RenderDevice.GetObjectsFactory().CreateModel();
	m_Mesh->AddConnection(m_Material, RenderDevice.GetPrimitivesFactory().CreateUIQuad(Size.x, Size.y));
}

CUITextureNode::~CUITextureNode()
{}



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
