#include "stdafx.h"

// General
#include "UICommonMaterial.h"

CUICommonMaterial::CUICommonMaterial(IRenderDevice& RenderDevice)
	: MaterialProxieT(RenderDevice.GetObjectsFactory().CreateMaterial("MaterialUIControl"))
{}

CUICommonMaterial::~CUICommonMaterial()
{}



//
// CMaterialUIControl
//
void CUICommonMaterial::SetColor(ColorRGBA color)
{
	MaterialData().DiffuseColor = color;
}

ColorRGBA CUICommonMaterial::GetColor() const
{
	return MaterialDataReadOnly().DiffuseColor;
}

void CUICommonMaterial::SetTexture(std::shared_ptr<ITexture> Texture)
{
	MaterialData().HasTextureDiffuse = (Texture != nullptr);

	if (MaterialDataReadOnly().HasTextureDiffuse)
		__super::SetTexture(0, Texture);
}

std::shared_ptr<ITexture> CUICommonMaterial::GetTexture() const
{
	return __super::GetTexture(0);
}
