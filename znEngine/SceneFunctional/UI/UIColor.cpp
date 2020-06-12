#include "stdafx.h"

// General
#include "UIColor.h"

CUIColorNode::CUIColorNode(IRenderDevice& RenderDevice, glm::vec2 Size)
	: m_Size(Size)
{
	m_Material = std::make_shared<UI_Color_Material>(RenderDevice);

	m_Mesh = RenderDevice.GetObjectsFactory().CreateModel();
	m_Mesh->AddConnection(m_Material, RenderDevice.GetPrimitivesFactory().CreateUIQuad(Size.x, Size.y));
}

CUIColorNode::~CUIColorNode()
{}



//
// CUIColorNode
//
void CUIColorNode::SetColor(glm::vec4 _color)
{
	m_Material->SetColor(_color);
}



//
// SceneNodeUI
//
glm::vec2 CUIColorNode::GetSize() const
{
    return m_Size;
}

void CUIColorNode::AcceptMesh(IVisitor* visitor)
{
}
