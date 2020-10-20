#include "stdafx.h"

// General
#include "UI_Color_Material.h"

UI_Color_Material::UI_Color_Material(IRenderDevice& RenderDevice) 
	: MaterialProxieT(RenderDevice, "UI_Color_Material")
{

}

UI_Color_Material::~UI_Color_Material()
{
}

void UI_Color_Material::SetColor(glm::vec4 color)
{
	MaterialData().Color = color;
	MarkMaterialDataDirty();
}

glm::vec4 UI_Color_Material::GetColor() const
{
	return MaterialDataReadOnly().Color;
}
