#pragma once

#include "UIBaseNode.h"
#include "UIText.h"
#include "Materials/UI_Button_Material.h"


class UIButtonClickEventArgs
{
public:
	UIButtonClickEventArgs()
	{}
};
typedef Delegate<UIButtonClickEventArgs> UIButtonClickEvent;


class CUIButtonNode : public CUIBaseNode
{
	typedef CUIBaseNode base;
public:
	enum ButtonState : uint32
	{
		Idle = 0,
		Hover = 1,
		Clicked = 2,
		Disabled = 3
	};

	CUIButtonNode();
	virtual ~CUIButtonNode();

	// CUIButtonNode
	void CreateDefault();

	// Input events
	virtual bool OnMouseButtonPressed(MouseButtonEventArgs& e) override;
	virtual void OnMouseButtonReleased(MouseButtonEventArgs& e) override;

	// Syntetic events
	virtual void OnMouseEntered() override;
	virtual void OnMouseLeaved() override;

	// CUIBaseNode
    virtual glm::vec2 GetSize() const override;

	virtual bool Accept(IVisitor& visitor) override;
	virtual bool AcceptMesh(IVisitor& visitor) override;

	UIButtonClickEvent Click;

private:
    glm::vec2                            m_Size;
	uint32                               m_State;
	std::shared_ptr<IMesh>               m_Mesh;
	std::shared_ptr<UI_Button_Material>  m_Material;
	std::shared_ptr<CUITextNode>         m_TextNode;
	
};