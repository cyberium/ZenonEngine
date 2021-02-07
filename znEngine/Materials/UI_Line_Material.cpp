#include "stdafx.h"

// General
#include "UI_Line_Material.h"

UI_Line_Material::UI_Line_Material(IRenderDevice& RenderDevice) 
	: MaterialProxieT(RenderDevice.GetObjectsFactory().CreateMaterial("UI_Line_Material"))
{
}

UI_Line_Material::~UI_Line_Material()
{}



//
// UI_Line_Material
//
void UI_Line_Material::SetColor(glm::vec4 Color)
{
	MaterialData().Color = Color;
	MarkMaterialDataDirty();
}

void UI_Line_Material::SetThickness(float Thickness)
{
	MaterialData().Thickness = Thickness;
    MarkMaterialDataDirty();
}

void UI_Line_Material::SetViewport(glm::vec2 Size)
{
	MaterialData().Viewport = Size;
    MarkMaterialDataDirty();
}

void UI_Line_Material::SetMiterLimit(float MiterLimit)
{
	MaterialData().MiterLimit = MiterLimit;
    MarkMaterialDataDirty();
}

void UI_Line_Material::SetSegmentsCount(uint32 SegmentsCount)
{
	MaterialData().SegmentsCount = SegmentsCount;
    MarkMaterialDataDirty();
}
