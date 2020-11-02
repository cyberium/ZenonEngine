#pragma once

#include "UIText.h"
#include "../../Materials/UI_Button_Material.h"


class ZN_API CUIButtonNode 
	: public CUIControl
{
public:
	enum ButtonState : uint32
	{
		Idle = 0,
		Hover = 1,
		Clicked = 2,
		Disabled = 3
	};

	CUIButtonNode(IRenderDevice& RenderDevice);
	virtual ~CUIButtonNode();

	// CUIButtonNode
	void CreateDefault();
	void SetText(const std::string& Text);

	// Input events
	virtual bool OnMouseButtonPressed(MouseButtonEventArgs& e) override;
	virtual void OnMouseButtonReleased(MouseButtonEventArgs& e) override;

	// Syntetic events
	virtual void OnMouseEntered() override;
	virtual void OnMouseLeaved() override;

	// CUIControl
    virtual glm::vec2 GetSize() const override;

	virtual void Accept(IVisitor* visitor) override;
	virtual void AcceptMesh(IVisitor* visitor) override;

private:
    glm::vec2                            m_Size;
	uint32                               m_State;
	std::shared_ptr<IModel>              m_Mesh;
	std::shared_ptr<UI_Button_Material>  m_Material;
	std::shared_ptr<IUIControl>        m_TextNode;
};
