#pragma once

#include "../../Materials/UI_Color_Material.h"

class ZN_API CUIColorNode 
	: public CUIBaseNode
{
public:
	CUIColorNode(IRenderDevice& RenderDevice, vec2 Size = vec2(1.0f, 1.0f));
	virtual ~CUIColorNode();

	// CUIColorNode
	void SetColor(vec4 _color);

	// CUIBaseNode
    virtual glm::vec2 GetSize() override;

	virtual void AcceptMesh(IVisitor* visitor) override;

private:
    glm::vec2                           m_Size;
	std::shared_ptr<IModel>             m_Mesh;
	std::shared_ptr<UI_Color_Material>  m_Material;
};