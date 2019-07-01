#pragma once

#include "SceneNodeUI.h"
#include "UIText.h"
#include "../../Materials/UI_Button_Material.h"


class OW_ENGINE_API UIButtonClickEventArgs
{
public:
	UIButtonClickEventArgs()
	{}
};
typedef Delegate<UIButtonClickEventArgs> UIButtonClickEvent;


class OW_ENGINE_API CUIButtonNode : public CUIBaseNode
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
    virtual glm::vec2 GetSize() override;

	virtual bool Accept(std::shared_ptr<IVisitor> visitor) override;
	virtual bool AcceptMesh(std::shared_ptr<IVisitor> visitor) override;

	UIButtonClickEvent Click;

private:
    glm::vec2                            m_Size;
	uint32                               m_State;
	std::shared_ptr<IMesh>               m_Mesh;
	std::shared_ptr<UI_Button_Material>  m_Material;
	std::shared_ptr<CUITextNode>         m_TextNode;
	
};