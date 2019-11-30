#include "stdafx.h"

// General
#include "UI_Button_Material.h"

// Additional
#include "Application.h"

UI_Button_Material::UI_Button_Material() :
	MaterialWrapper(_RenderDevice->CreateMaterial(sizeof(MaterialProperties)))
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	// CreateShaders
	std::shared_ptr<IShader> g_pVertexShader = _RenderDevice->CreateShader(
		IShader::VertexShader, "IDB_SHADER_UI_BUTTON", IShader::ShaderMacros(), "VS_main", "latest"
	);
    g_pVertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> g_pPixelShader = _RenderDevice->CreateShader(
		IShader::PixelShader, "IDB_SHADER_UI_BUTTON", IShader::ShaderMacros(), "PS_main", "latest"
	);

	// Create samplers
	std::shared_ptr<ISamplerState> g_LinearClampSampler = _RenderDevice->CreateSamplerState();
	g_LinearClampSampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	g_LinearClampSampler->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);

	g_pPixelShader->GetShaderParameterByName("DiffuseTextureSampler")->Set(g_LinearClampSampler.get());

	// Material
	SetShader(IShader::VertexShader, g_pVertexShader);
	SetShader(IShader::PixelShader, g_pPixelShader);
}

UI_Button_Material::~UI_Button_Material()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

void UI_Button_Material::SetIdleTexture(std::shared_ptr<ITexture> _texture)
{
	base::SetTexture(0, _texture);
	MarkConstantBufferDirty();
}

void UI_Button_Material::SetHoverTexture(std::shared_ptr<ITexture> _texture)
{
	base::SetTexture(1, _texture);
	MarkConstantBufferDirty();
}

void UI_Button_Material::SetClickedTexture(std::shared_ptr<ITexture> _texture)
{
	base::SetTexture(2, _texture);
	MarkConstantBufferDirty();
}

void UI_Button_Material::SetDisabledTexture(std::shared_ptr<ITexture> _texture)
{
	base::SetTexture(3, _texture);
	MarkConstantBufferDirty();
}

void UI_Button_Material::SetColor(vec4 color)
{
	m_pProperties->Color = color;
	MarkConstantBufferDirty();
}

void UI_Button_Material::SetState(uint32 state)
{
	m_pProperties->State = state;
	MarkConstantBufferDirty();
}


void UI_Button_Material::UpdateConstantBuffer() const
{
	base::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}
