#include "stdafx.h"

// General
#include "UIColor.h"

CUIColorNode::CUIColorNode(glm::vec2 Size)
	: m_Size(Size)
{

}

CUIColorNode::~CUIColorNode()
{}

void CUIColorNode::Initialize()
{
	m_Material = std::make_shared<UI_Color_Material>(GetBaseManager().GetApplication().GetRenderDevice());

	m_Mesh = GetBaseManager().GetApplication().GetRenderDevice().GetObjectsFactory().CreateModel();
	m_Mesh->AddConnection(m_Material, GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(m_Size.x, m_Size.y));

	m_ColorProperty = std::make_shared<CPropertyWrapped<glm::vec4>>();
	m_ColorProperty->SetName("Color");
	m_ColorProperty->SetValueSetter(std::bind(&CUIColorNode::SetColor, this, std::placeholders::_1));
	m_ColorProperty->SetValueGetter(std::bind(&CUIColorNode::GetColor, this));
	GetProperties()->AddProperty(m_ColorProperty);
}



//
// CUIColorNode
//
void CUIColorNode::SetColor(glm::vec4 _color)
{
	m_Material->SetColor(_color);
}

glm::vec4 CUIColorNode::GetColor() const
{
	return m_Material->GetColor();
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
	m_Mesh->Accept(visitor);
}
