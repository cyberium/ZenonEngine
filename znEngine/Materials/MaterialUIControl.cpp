#include "stdafx.h"

// General
#include "MaterialUIControl.h"

CMaterialUIControl::CMaterialUIControl(IRenderDevice& RenderDevice)
	: MaterialProxieT(RenderDevice.GetObjectsFactory().CreateMaterial("MaterialUIControl"))
{}

CMaterialUIControl::~CMaterialUIControl()
{}



//
// CMaterialUIControl
//
void CMaterialUIControl::SetColor(glm::vec4 color)
{
	MaterialData().DiffuseColor = color;
	MarkMaterialDataDirty();
}

glm::vec4 CMaterialUIControl::GetColor() const
{
	return MaterialDataReadOnly().DiffuseColor;
}

void CMaterialUIControl::SetTexture(std::shared_ptr<ITexture> Texture)
{
	MaterialData().HasTextureDiffuse = (Texture != nullptr);
	MarkMaterialDataDirty();

	if (MaterialData().HasTextureDiffuse)
		__super::SetTexture(0, Texture);
}

std::shared_ptr<ITexture> CMaterialUIControl::GetTexture() const
{
	return __super::GetTexture(0);
}
