#pragma once

#include "../../Materials/UI_Texture_Material.h"

class ZN_API CUITextureNode 
	: public CUIControl
{
public:
	CUITextureNode(IScene& Scene);
	virtual ~CUITextureNode();

	void Initialize() override;

	// CUITextureNode
	void SetTexture(std::shared_ptr<ITexture> _texture);
	void SetColor(glm::vec4 _color);

	// CUIControl
    virtual glm::vec2 GetSize() const override;

	virtual void AcceptMesh(IVisitor* visitor) override;

private:
    glm::vec2                               m_Size;
	std::shared_ptr<IModel>                 m_Mesh;
	std::shared_ptr<UI_Texture_Material>    m_Material;
};