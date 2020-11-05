#include "stdafx.h"

// General
#include "UIColor.h"

CUIColorNode::CUIColorNode(IScene& Scene)
	: CUIControl(Scene)
{}

CUIColorNode::~CUIColorNode()
{}



void CUIColorNode::Initialize()
{
	__super::Initialize();

	m_Material = MakeShared(UI_Color_Material, GetBaseManager().GetApplication().GetRenderDevice());

	auto geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(1.0f, 1.0f);
	geom->SetBounds(BoundingBox(glm::vec3(-100.0f), glm::vec3(100.0f)));

	m_Mesh = GetBaseManager().GetApplication().GetRenderDevice().GetObjectsFactory().CreateModel();
	m_Mesh->AddConnection(m_Material, geom);

	m_ColorProperty = MakeShared(CPropertyWrapped<glm::vec4>);
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
// CUIControl
//
glm::vec2 CUIColorNode::GetSize() const
{
    return m_Size;
}

void CUIColorNode::AcceptMesh(IVisitor* visitor)
{
	m_Mesh->Accept(visitor);
}
