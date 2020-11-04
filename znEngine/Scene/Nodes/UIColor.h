#pragma once

#include "../../Materials/UI_Color_Material.h"

class ZN_API CUIColorNode 
	: public CUIControl
{
public:
	CUIColorNode(IScene& Scene);
	virtual ~CUIColorNode();

	void Initialize() override;

	// CUIColorNode
	void SetColor(glm::vec4 _color);
	glm::vec4 GetColor() const;

	// CUIControl
    virtual glm::vec2 GetSize() const override;

	virtual void AcceptMesh(IVisitor* visitor) override;

private:
    glm::vec2                           m_Size;
	std::shared_ptr<IModel>             m_Mesh;
	std::shared_ptr<UI_Color_Material>  m_Material;
	std::shared_ptr<CPropertyWrapped<glm::vec4>> m_ColorProperty;
};