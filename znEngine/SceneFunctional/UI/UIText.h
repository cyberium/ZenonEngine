#pragma once

#include "../../Formats/Fonts/Font.h"
#include "../../Materials/UI_Font_Material.h"

class ZN_API CUITextNode 
	: public SceneNodeUI
{
public:
	CUITextNode();
	virtual ~CUITextNode();

	void						Initialize() override;

	// CUITextNode
    void                        SetFont(std::shared_ptr<CFont> _font);
    std::shared_ptr<CFont>      GetFont() const;
	const std::shared_ptr<UI_Font_Material>& GetMaterial() const;
    void                        SetTextColor(cvec4 _color);
    glm::vec2                   GetTextSize() const;

	const std::string         GetText() const;
	const glm::vec2           GetOffset() const;

	// SceneNodeUI
    virtual glm::vec2           GetSize() override;

private:
	std::shared_ptr<CFont>             m_Font;
	std::shared_ptr<UI_Font_Material>  m_Material;

	std::shared_ptr<IPropertyT<std::string>> m_TextProperty;
	std::shared_ptr<IPropertyT<glm::vec2>>   m_OffsetProperty;
};