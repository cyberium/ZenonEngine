#include "stdafx.h"

// General
#include "UI_Texture_Material.h"

UI_Texture_Material::UI_Texture_Material(IRenderDevice& RenderDevice) 
	: UI_Color_Material(RenderDevice)
{

	// CreateShaders
	const auto& g_pVertexShader = RenderDevice.GetObjectsFactory().CreateShader(
		EShaderType::VertexShader, "IDB_SHADER_UI_TEXTURE", "VS_main", IShader::ShaderMacros(), "latest"
	);
    g_pVertexShader->LoadInputLayoutFromReflector();

	const auto& g_pPixelShader = RenderDevice.GetObjectsFactory().CreateShader(
		EShaderType::PixelShader, "IDB_SHADER_UI_TEXTURE", "PS_main", IShader::ShaderMacros(), "latest"
	);

	// Create samplers
	const auto& g_LinearClampSampler = RenderDevice.GetObjectsFactory().CreateSamplerState();
	g_LinearClampSampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	g_LinearClampSampler->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);

    SetSampler(0, g_LinearClampSampler);
}

UI_Texture_Material::~UI_Texture_Material()
{
}

void UI_Texture_Material::SetTexture(const std::shared_ptr<ITexture> _texture)
{
	__super::SetTexture(0, _texture);
	MarkMaterialDataDirty();
}
