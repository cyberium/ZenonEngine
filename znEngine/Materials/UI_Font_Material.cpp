#include "stdafx.h"

// General
#include "UI_Font_Material.h"

UI_Font_Material::UI_Font_Material(IRenderDevice& RenderDevice) 
	: MaterialProxieT(RenderDevice, "UI_Font_Material")
{}

UI_Font_Material::~UI_Font_Material()
{}



//
// UI_Font_Material
//
void UI_Font_Material::SetColor(glm::vec4 color)
{
	MaterialData().Color = color;
	MarkMaterialDataDirty();
}


void UI_Font_Material::SetOffset(glm::vec2 offset)
{
	MaterialData().Offset = offset;
	MarkMaterialDataDirty();
}

void UI_Font_Material::SetSelected(bool color)
{
	MaterialData().IsSelected = color;
	MarkMaterialDataDirty();
}