#pragma once

class ZN_API CUIControlText
	: public CUIControl
	, public IUIControlText
{
public:
	CUIControlText(IScene& Scene);
	virtual ~CUIControlText();

	void                                            Initialize() override;

	// IUIControlText
	void SetFont(std::shared_ptr<IznFont> Font) override;
	std::shared_ptr<IznFont> GetFont() const override;
	void SetText(std::string Text) override;
	std::string GetText() const override;
	void SetOffset(glm::vec2 Offset) override;
	glm::vec2 GetOffset() const override;
	void SetColor(glm::vec4 Color) override;
	glm::vec4 GetColor() const override;

private:
	std::shared_ptr<IznFont>                        m_Font;
	std::shared_ptr<IPropertyT<std::string>>        m_TextProperty;
	std::shared_ptr<IPropertyT<glm::vec2>>          m_OffsetProperty;
	std::shared_ptr<IPropertyT<glm::vec4>>          m_ColorProperty;
};