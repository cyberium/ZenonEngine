#pragma once

class ZN_API CUITextNode
	: public CUIControl
{
public:
	CUITextNode(IScene& Scene);
	virtual ~CUITextNode();

	void                                            Initialize() override;

	// CUITextNode
	std::shared_ptr<IznFont>                        GetFont() const;
	std::string                                     GetText() const;
	glm::vec2                                       GetOffset() const;
	glm::vec4                                       GetColor() const;

	// CUIControl
	virtual glm::vec2                               GetSize() const override;

private:
	std::shared_ptr<IznFont>                        m_Font;
	std::shared_ptr<IPropertyT<std::string>>        m_TextProperty;
	std::shared_ptr<IPropertyT<glm::vec2>>          m_OffsetProperty;
	std::shared_ptr<IPropertyT<glm::vec4>>          m_ColorProperty;
};