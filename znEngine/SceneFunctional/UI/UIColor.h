#pragma once

#include "../../Materials/UI_Color_Material.h"

class ZN_API CUIColorNode : public CUIBaseNode
{
	typedef CUIBaseNode base;
public:
	CUIColorNode(IRenderDevice* RenderDevice, vec2 Size = vec2(1.0f, 1.0f));
	virtual ~CUIColorNode();

	// CUIColorNode
	void SetColor(vec4 _color);

	// CUIBaseNode
    virtual glm::vec2 GetSize() override;

	virtual bool AcceptMesh(IVisitor* visitor) override;

private:
    glm::vec2                           m_Size;
	IMesh*              m_Mesh;
	std::shared_ptr<UI_Color_Material>  m_Material;
};