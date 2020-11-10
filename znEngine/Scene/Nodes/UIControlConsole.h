#pragma once

class ZN_API CUIControlConsole
	: public CUIControl
{
public:
	CUIControlConsole(IScene& Scene);
	virtual ~CUIControlConsole();

	void                                            Initialize() override;

	// CUITextNode
	std::shared_ptr<IznFont>                        GetFont() const;
	std::string                                     GetText() const;
	glm::vec2                                       GetOffset() const;
	glm::vec4                                       GetColor() const;

	// CUIControl
	glm::vec2                                       GetSize() const override;

private:
	std::shared_ptr<IznFont>                        m_Font;
	std::shared_ptr<IPropertyT<std::string>>        m_TextProperty;
	std::shared_ptr<IPropertyT<glm::vec2>>          m_OffsetProperty;
	std::shared_ptr<IPropertyT<glm::vec4>>          m_ColorProperty;
};