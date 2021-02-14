#pragma once

#include "../../EngineIntfs.h"
#include "UITextMaterial.h"
#include "UITextGeometry.h"

class ZN_API CUITextModel
	: public ModelProxie
{
public:
	CUITextModel(IRenderDevice& RenderDevice, std::shared_ptr<IznFont> Font);
	virtual ~CUITextModel();
	
	void SetText(std::string Text);
	std::string GetText() const;
	void SetTextAlignHorizontal(ETextAlignHorizontal TextAlignHorizontal);
	ETextAlignHorizontal GetTextAlignHorizontal() const;
	void SetTextAlignVertical(ETextAlignVertical TextAlignVertical);
	ETextAlignVertical GetTextAlignVertical() const;
	void SetColor(ColorRGBA Color);
	ColorRGBA GetColor() const;

	// IModel
	bool Render(const ShaderMap& Shaders) const override;

private:
	std::shared_ptr<IznFont> m_Font;
	std::shared_ptr<CUITextMaterial> m_Material;
	std::shared_ptr<CUITextGeometry> m_Geometry;

	std::shared_ptr<IPropertyT<std::string>> m_TextProperty;
	std::shared_ptr<IPropertyT<ETextAlignHorizontal>> m_TextAlignHorizontalProperty;
	std::shared_ptr<IPropertyT<ETextAlignVertical>> m_TextAlignVerticalProperty;
	std::shared_ptr<IPropertyT<ColorRGBA>> m_ColorProperty;
};