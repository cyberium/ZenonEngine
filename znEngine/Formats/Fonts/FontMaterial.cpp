#include "stdafx.h"

// General
#include "FontMaterial.h"

CFontMaterial::CFontMaterial(const IRenderDevice & RenderDevice)
	: MaterialProxieT<SGPUPerCharacterDataPS>(RenderDevice.GetObjectsFactory().CreateMaterial("FontMaterial"))
{
}

CFontMaterial::~CFontMaterial()
{
}

void CFontMaterial::SetTexture(const std::shared_ptr<ITexture>& Texture)
{
	__super::SetTexture(0, Texture);
}

std::shared_ptr<ITexture> CFontMaterial::GetTexture() const
{
	return __super::GetTexture(0);
}

void CFontMaterial::SetColor(const ColorRGBA& Color)
{
	MaterialData().Color = Color;
}

ColorRGBA CFontMaterial::GetColor() const
{
	return MaterialDataReadOnly().Color;
}
