#include "stdafx.h"

// General
#include "UIButton.h"

namespace
{
	const char* cDefaultText = "<empty>";
	const glm::vec4  cDefaultColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

CUIButtonNode::CUIButtonNode(IRenderDevice& RenderDevice)
	: m_State(Idle)
{
	m_Material = std::make_shared<UI_Button_Material>(RenderDevice);
}

CUIButtonNode::~CUIButtonNode()
{}



//
// CUIButtonNode
//
void CUIButtonNode::CreateDefault()
{
	m_Material->SetIdleTexture(GetBaseManager().GetApplication().GetRenderDevice().GetObjectsFactory().LoadTexture2D("btn_idle.png"));
	m_Material->SetHoverTexture(GetBaseManager().GetApplication().GetRenderDevice().GetObjectsFactory().LoadTexture2D("btn_hover.png"));
	m_Material->SetClickedTexture(GetBaseManager().GetApplication().GetRenderDevice().GetObjectsFactory().LoadTexture2D("btn_clicked.png"));
	m_Material->SetDisabledTexture(GetBaseManager().GetApplication().GetRenderDevice().GetObjectsFactory().LoadTexture2D("btn_idle.png"));

	const auto& idleTexture = m_Material->GetTexture(0);
    m_Size = idleTexture->GetSize();

	auto geometry = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(idleTexture->GetWidth(), idleTexture->GetHeight());
	
	m_Mesh = GetBaseManager().GetApplication().GetRenderDevice().GetObjectsFactory().CreateModel();
	m_Mesh->AddConnection(m_Material, geometry);

    m_TextNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(GetScene(), cSceneNodeUI_Text, shared_from_this());
	m_TextNode->GetProperties()->GetPropertyT<std::string>("Text")->Set(cDefaultText);
	m_TextNode->SetTranslate(glm::vec2(10.0f, 10.0f));
	std::static_pointer_cast<CUITextNode>(m_TextNode)->SetTextColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void CUIButtonNode::SetText(const std::string& Text)
{
	m_TextNode->GetProperties()->GetPropertyT<std::string>("Text")->Set(Text);
}



//
// Input events
//
bool CUIButtonNode::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	m_State = Clicked;

	return __super::OnMouseButtonPressed(e);
}

void CUIButtonNode::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	if (IsPointInBoundsAbs(e.GetPoint()))
	{
		m_State = Hover;
		return;
	}

	m_State = Idle;
}



//
// Syntetic events
//
void CUIButtonNode::OnMouseEntered()
{
	m_State = Hover;
}

void CUIButtonNode::OnMouseLeaved()
{
	m_State = Idle;
}





//
// SceneNodeUI
//
glm::vec2 CUIButtonNode::GetSize() const
{
    return m_Size;
}

void CUIButtonNode::Accept(IVisitor* visitor)
{
	SceneNodeUI::Accept(visitor);
}

void CUIButtonNode::AcceptMesh(IVisitor* visitor)
{
	m_Material->SetState(m_State);
	
	return m_Mesh->Accept(visitor);
}
