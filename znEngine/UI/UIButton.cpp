#include "stdafx.h"

// General
#include "UIButton.h"

// Additional
#include "Application.h"

namespace
{
	const char* cDefaultText = "<empty>";
	const vec4  cDefaultColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

CUIButtonNode::CUIButtonNode()
	: m_State(Idle)
{
	m_Material = std::make_shared<UI_Button_Material>();
	m_Material->SetWrapper(m_Material);


}

CUIButtonNode::~CUIButtonNode()
{
}



//
// CUIButtonNode
//
void CUIButtonNode::CreateDefault()
{
	m_Material->SetIdleTexture(_RenderDevice->CreateTexture2D("Textures\\btn_idle.png"));
	m_Material->SetHoverTexture(_RenderDevice->CreateTexture2D("Textures\\btn_hover.png"));
	m_Material->SetClickedTexture(_RenderDevice->CreateTexture2D("Textures\\btn_clicked.png")); 
	m_Material->SetDisabledTexture(_RenderDevice->CreateTexture2D("Textures\\btn_disabled.png"));

	std::shared_ptr<Texture> idleTexture = m_Material->GetTexture(0);
    m_Size = idleTexture->GetSize();

	m_Mesh = _RenderDevice->CreateUIQuad(idleTexture->GetWidth(), idleTexture->GetHeight());

	m_TextNode = std::make_shared<CUITextNode>();
	m_TextNode->SetParentInternal(weak_from_this());
	m_TextNode->SetText(cDefaultText);
	m_TextNode->SetTranslate(vec2(10.0f, 10.0f));
	m_TextNode->SetTextColor(vec4(0.0f, 0.0f, 1.0f, 1.0f));
}



//
// Input events
//
bool CUIButtonNode::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	m_State = Clicked;

	// Raise event
	UIButtonClickEventArgs args;
	Click(args);

	return true;
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
// CUIBaseNode
//
glm::vec2 CUIButtonNode::GetSize() const
{
    return m_Size;
}

bool CUIButtonNode::Accept(IVisitor & visitor)
{
	bool visitResult = base::Accept(visitor);
	if (!visitResult)
		return false;

	if (m_TextNode != nullptr)
	{
		m_TextNode->Accept(visitor);
	}

	return true;
}

bool CUIButtonNode::AcceptMesh(IVisitor& visitor)
{
	m_Material->SetState(m_State);
	m_Mesh->SetMaterial(m_Material);

	return m_Mesh->Accept(visitor);
}
