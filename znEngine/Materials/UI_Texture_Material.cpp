#include "stdafx.h"

// General
#include "UI_Texture_Material.h"

// Additional
#include "Application.h"

UI_Texture_Material::UI_Texture_Material() :
	MaterialWrapper(_RenderDevice->CreateMaterial(sizeof(MaterialProperties)))
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	// CreateShaders
	std::shared_ptr<IShader> g_pVertexShader = _RenderDevice->CreateShader(
		IShader::ShaderType::VertexShader, "IDB_SHADER_UI_TEXTURE", IShader::ShaderMacros(), "VS_main", "latest"
	);
    g_pVertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> g_pPixelShader = _RenderDevice->CreateShader(
		IShader::ShaderType::PixelShader, "IDB_SHADER_UI_TEXTURE", IShader::ShaderMacros(), "PS_main", "latest"
	);

	// Create samplers
	std::shared_ptr<ISamplerState> g_LinearClampSampler = _RenderDevice->CreateSamplerState();
	g_LinearClampSampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	g_LinearClampSampler->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);

    SetSampler(0, g_LinearClampSampler);

	// Material
	SetShader(IShader::ShaderType::VertexShader, g_pVertexShader);
	SetShader(IShader::ShaderType::PixelShader, g_pPixelShader);
}

UI_Texture_Material::~UI_Texture_Material()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

void UI_Texture_Material::SetTexture(std::shared_ptr<ITexture> _texture)
{
	base::SetTexture(0, _texture);
	MarkConstantBufferDirty();
}

void UI_Texture_Material::SetColor(vec4 color)
{
	m_pProperties->Color = color;
	MarkConstantBufferDirty();
}


void UI_Texture_Material::UpdateConstantBuffer() const
{
	base::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}
