#include "stdafx.h"

// General
#include "UI_Color_Material.h"

UI_Color_Material::UI_Color_Material(IRenderDevice& RenderDevice) 
	: MaterialProxieT(RenderDevice)
{
	// CreateShaders
	/*const auto& g_pVertexShader = RenderDevice.GetObjectsFactory().CreateShader(
		EShaderType::VertexShader, "IDB_SHADER_UI_COLOR", "VS_main", IShader::ShaderMacros(), "latest"
	);
    g_pVertexShader->LoadInputLayoutFromReflector();

	const auto& g_pPixelShader = RenderDevice.GetObjectsFactory().CreateShader(
		EShaderType::PixelShader, "IDB_SHADER_UI_COLOR", "PS_main", IShader::ShaderMacros(), "latest"
	);*/
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
