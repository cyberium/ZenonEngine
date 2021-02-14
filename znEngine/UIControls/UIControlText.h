#pragma once

#include "Text/UITextModel.h"

class ZN_API CUIControlText
	: public CUIControl
	, public IUIControlText
{
public:
	CUIControlText(IScene& Scene);
	virtual ~CUIControlText();

	void Initialize() override;

	// IUIControlText
	void SetFont(std::shared_ptr<IznFont> Font) override;
	std::shared_ptr<IznFont> GetFont() const override;
	void SetText(std::string Text) override;
	std::string GetText() const override;
	void SetTextAlignHorizontal(ETextAlignHorizontal TextAlignHorizontal) override;
	ETextAlignHorizontal GetTextAlignHorizontal() const override;
	void SetTextAlignVertical(ETextAlignVertical TextAlignVertical) override;
	ETextAlignVertical GetTextAlignVertical() const override;
	void SetColor(ColorRGBA Color) override;
	ColorRGBA GetColor() const override;

protected:
	EVisitResult AcceptContent(IVisitor * visitor) override;

private:
	std::shared_ptr<IznFont> m_Font;
	std::shared_ptr<CUITextModel> m_FontModel;
};