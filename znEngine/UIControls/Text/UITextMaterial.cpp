#include "stdafx.h"

// General
#include "UITextMaterial.h"

CUITextMaterial::CUITextMaterial(const IRenderDevice & RenderDevice)
	: MaterialProxieT<SGPUPerCharacterDataPS>(RenderDevice.GetObjectsFactory().CreateMaterial("FontMaterial"))
{
}

CUITextMaterial::~CUITextMaterial()
{
}

void CUITextMaterial::SetTexture(const std::shared_ptr<ITexture>& Texture)
{
	__super::SetTexture(0, Texture);
}

std::shared_ptr<ITexture> CUITextMaterial::GetTexture() const
{
	return __super::GetTexture(0);
}

void CUITextMaterial::SetColor(const ColorRGBA& Color)
{
	MaterialData().Color = Color;
}

ColorRGBA CUITextMaterial::GetColor() const
{
	return MaterialDataReadOnly().Color;
}
