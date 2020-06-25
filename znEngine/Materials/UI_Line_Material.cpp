#include "stdafx.h"

// General
#include "UI_Line_Material.h"

UI_Line_Material::UI_Line_Material(IRenderDevice& RenderDevice) 
	: MaterialProxieT(RenderDevice)
{
	// CreateShaders
	const auto& g_pVertexShader = RenderDevice.GetObjectsFactory().CreateShader(
		EShaderType::VertexShader, "IDB_SHADER_UI_LINE", "VS_main", IShader::ShaderMacros(), "latest"
	);
    g_pVertexShader->LoadInputLayoutFromReflector();

	const auto& g_pGeomShader = RenderDevice.GetObjectsFactory().CreateShader(
        EShaderType::GeometryShader, "IDB_SHADER_UI_LINE", "GS_main", IShader::ShaderMacros(), "latest"
    );

	const auto& g_pPixelShader = RenderDevice.GetObjectsFactory().CreateShader(
		EShaderType::PixelShader, "IDB_SHADER_UI_LINE", "PS_LineV2", IShader::ShaderMacros(), "latest"
	);
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
