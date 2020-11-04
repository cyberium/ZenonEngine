#include "stdafx.h"

// General
#include "UI_Texture_Material.h"

UI_Texture_Material::UI_Texture_Material(IRenderDevice& RenderDevice) 
	: UI_Color_Material(RenderDevice)
{
}

UI_Texture_Material::~UI_Texture_Material()
{
}

void UI_Texture_Material::SetTexture(const std::shared_ptr<ITexture> _texture)
{
	__super::SetTexture(0, _texture);
	MarkMaterialDataDirty();
}
