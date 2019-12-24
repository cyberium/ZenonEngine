#pragma once

#include "Fonts/Font.h"
#include "../../Materials/UI_Font_Material.h"

class OW_ENGINE_API CUITextNode : public CUIBaseNode
{
	typedef CUIBaseNode base;
public:
	CUITextNode();
	virtual ~CUITextNode();

	void						Initialize() override;

	// CUITextNode
    void                        SetFont(std::shared_ptr<CFontMesh> _font);
    std::shared_ptr<CFontMesh>  GetFont() const;

    void                        SetTextColor(cvec4 _color);
    glm::vec2                   GetTextSize() const;

	// CUIBaseNode
    virtual glm::vec2           GetSize() override;

    virtual bool                AcceptMesh(IVisitor* visitor) override;

private:
	std::shared_ptr<CFontMesh>         m_Font;
	std::shared_ptr<UI_Font_Material>  m_Material;

	std::shared_ptr<IPropertyT<std::string>> m_TextProperty;
	std::shared_ptr<IPropertyT<glm::vec2>>   m_OffsetProperty;
};