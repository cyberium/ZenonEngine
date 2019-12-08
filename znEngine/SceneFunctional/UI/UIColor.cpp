#include "stdafx.h"

// General
#include "UIColor.h"

CUIColorNode::CUIColorNode(std::shared_ptr<IRenderDevice> RenderDevice, vec2 Size)
	: m_Size(Size)
{
	m_Material = std::make_shared<UI_Color_Material>(RenderDevice);
	m_Material->SetWrapper(m_Material);;

	m_Mesh = RenderDevice->GetPrimitiveCollection()->CreateUIQuad(Size.x, Size.y);
	m_Mesh->SetMaterial(m_Material);
}

CUIColorNode::~CUIColorNode()
{}



//
// CUIColorNode
//
void CUIColorNode::SetColor(vec4 _color)
{
	m_Material->SetColor(_color);
}



//
// CUIBaseNode
//
glm::vec2 CUIColorNode::GetSize()
{
    return m_Size;
}

bool CUIColorNode::AcceptMesh(IVisitor* visitor)
{
	return m_Mesh->Accept(visitor);
}
