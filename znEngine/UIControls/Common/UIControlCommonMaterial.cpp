#include "stdafx.h"

// General
#include "UIControlCommonMaterial.h"

CUIControlCommonMaterial::CUIControlCommonMaterial(IRenderDevice& RenderDevice)
	: MaterialProxieT(RenderDevice.GetObjectsFactory().CreateMaterial("MaterialUIControl"))
{}

CUIControlCommonMaterial::~CUIControlCommonMaterial()
{}



//
// CMaterialUIControl
//
void CUIControlCommonMaterial::SetColor(glm::vec4 color)
{
	MaterialData().DiffuseColor = color;
}

glm::vec4 CUIControlCommonMaterial::GetColor() const
{
	return MaterialDataReadOnly().DiffuseColor;
}

void CUIControlCommonMaterial::SetTexture(std::shared_ptr<ITexture> Texture)
{
	MaterialData().HasTextureDiffuse = (Texture != nullptr);

	if (MaterialDataReadOnly().HasTextureDiffuse)
		__super::SetTexture(0, Texture);
}

std::shared_ptr<ITexture> CUIControlCommonMaterial::GetTexture() const
{
	return __super::GetTexture(0);
}
