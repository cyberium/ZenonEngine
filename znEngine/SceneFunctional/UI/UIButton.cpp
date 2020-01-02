#include "stdafx.h"

// General
#include "UIButton.h"

namespace
{
	const char* cDefaultText = "<empty>";
	const vec4  cDefaultColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

CUIButtonNode::CUIButtonNode(std::shared_ptr<IRenderDevice> RenderDevice)
	: m_State(Idle)
{
	m_Material = std::make_shared<UI_Button_Material>(RenderDevice);
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
	m_Material->SetIdleTexture(GetBaseManager()->GetManager<IRenderDevice>()->CreateTexture2D("Textures\\btn_idle.png"));
	m_Material->SetHoverTexture(GetBaseManager()->GetManager<IRenderDevice>()->CreateTexture2D("Textures\\btn_hover.png"));
	m_Material->SetClickedTexture(GetBaseManager()->GetManager<IRenderDevice>()->CreateTexture2D("Textures\\btn_clicked.png"));
	m_Material->SetDisabledTexture(GetBaseManager()->GetManager<IRenderDevice>()->CreateTexture2D("Textures\\btn_disabled.png"));

	std::shared_ptr<ITexture> idleTexture = m_Material->GetTexture(0);
    m_Size = idleTexture->GetSize();

	m_Mesh = GetBaseManager()->GetManager<IRenderDevice>()->GetPrimitiveCollection()->CreateUIQuad(idleTexture->GetWidth(), idleTexture->GetHeight());
	m_Mesh->SetMaterial(m_Material);

    m_TextNode = CreateSceneNode<CUITextNode>();
	m_TextNode->GetProperties()->GetPropertyT<std::string>("Text")->Set(cDefaultText);
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
glm::vec2 CUIButtonNode::GetSize()
{
    return m_Size;
}

bool CUIButtonNode::Accept(IVisitor* visitor)
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

bool CUIButtonNode::AcceptMesh(IVisitor* visitor)
{
	m_Material->SetState(m_State);
	
	return m_Mesh->Accept(visitor);
}
