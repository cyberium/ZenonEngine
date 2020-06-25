#include "stdafx.h"

// General
#include "UI_Button_Material.h"

UI_Button_Material::UI_Button_Material(IRenderDevice& RenderDevice) :
	MaterialProxieT(RenderDevice)
{}

UI_Button_Material::~UI_Button_Material()
{}

void UI_Button_Material::SetIdleTexture(std::shared_ptr<ITexture> _texture)
{
	__super::SetTexture(0, _texture);
	MarkMaterialDataDirty();
}

void UI_Button_Material::SetHoverTexture(std::shared_ptr<ITexture> _texture)
{
	__super::SetTexture(1, _texture);
	MarkMaterialDataDirty();
}

void UI_Button_Material::SetClickedTexture(std::shared_ptr<ITexture> _texture)
{
	__super::SetTexture(2, _texture);
	MarkMaterialDataDirty();
}

void UI_Button_Material::SetDisabledTexture(std::shared_ptr<ITexture> _texture)
{
	__super::SetTexture(3, _texture);
	MarkMaterialDataDirty();
}

void UI_Button_Material::SetColor(glm::vec4 color)
{
	MaterialData().Color = color;
	MarkMaterialDataDirty();
}

void UI_Button_Material::SetState(uint32 state)
{
	MaterialData().State = state;
	MarkMaterialDataDirty();
}
