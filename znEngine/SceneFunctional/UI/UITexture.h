#pragma once

#include "../../Materials/UI_Texture_Material.h"

class ZN_API CUITextureNode : public CUIBaseNode
{
	typedef CUIBaseNode base;
public:
	CUITextureNode(IRenderDevice* RenderDevice, vec2 Size = vec2(1.0f, 1.0f));
	virtual ~CUITextureNode();

	// CUITextureNode
	void SetTexture(ITexture* _texture);
	void SetColor(vec4 _color);

	// CUIBaseNode
    virtual glm::vec2 GetSize() override;

	virtual bool AcceptMesh(IVisitor* visitor) override;

private:
    glm::vec2                               m_Size;
	IMesh*                  m_Mesh;
	std::shared_ptr<UI_Texture_Material>    m_Material;
};