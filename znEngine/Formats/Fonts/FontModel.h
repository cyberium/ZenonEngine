#pragma once

#include "EngineIntfs.h"
#include "FontMaterial.h"
#include "FontGeometry.h"

class CFontModel
	: public ModelProxie
{
public:
	CFontModel(IRenderDevice& RenderDevice);
	virtual ~CFontModel();
	
	// IUIControlText
	void SetText(std::string Text);
	std::string GetText() const;
	void SetTextAlignHorizontal(ETextAlignHorizontal TextAlignHorizontal);
	ETextAlignHorizontal GetTextAlignHorizontal() const;
	void SetTextAlignVertical(ETextAlignVertical TextAlignVertical);
	ETextAlignVertical GetTextAlignVertical() const;
	void SetColor(ColorRGBA Color);
	ColorRGBA GetColor() const;

private:
	std::shared_ptr<CFontMaterial> m_Material;
	std::shared_ptr<CFontGeometry> m_Geometry;

	std::shared_ptr<IPropertyT<std::string>>        m_TextProperty;
	std::shared_ptr<IPropertyT<ETextAlignHorizontal>> m_TextAlignHorizontalProperty;
	std::shared_ptr<IPropertyT<ETextAlignVertical>> m_TextAlignVerticalProperty;
	std::shared_ptr<IPropertyT<ColorRGBA>>          m_ColorProperty;
};