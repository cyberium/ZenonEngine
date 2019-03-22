#pragma once

#include "UIBaseNode.h"
#include "Fonts/Font.h"
#include "Materials/UI_Font_Material.h"

class CUITextNode : public CUIBaseNode
{
	typedef CUIBaseNode base;
public:
	CUITextNode();
	virtual ~CUITextNode();

	// CUITextNode
    void                        SetFont(std::shared_ptr<CFontMesh> _font);
    std::shared_ptr<CFontMesh>  GetFont() const;

    void                        SetText(const std::string& _text);
    void                        SetTextColor(cvec4 _color);
    std::string                 GetText() const;
    void                        SetOffset(glm::vec2 Offset);
    glm::vec2                   GetOffset() const;
    glm::vec2                   GetTextSize() const;

	// CUIBaseNode
    virtual glm::vec2           GetSize() const override;

    virtual bool                AcceptMesh(IVisitor& visitor) override;

private:
	std::shared_ptr<CFontMesh>         m_Font;
	std::shared_ptr<UI_Font_Material>  m_Material;
	std::string                        m_Text;
	glm::vec2                          m_Offset;
};