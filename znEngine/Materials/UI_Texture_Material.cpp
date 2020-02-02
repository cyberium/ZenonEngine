#include "stdafx.h"

// General
#include "UI_Texture_Material.h"

UI_Texture_Material::UI_Texture_Material(IRenderDevice& RenderDevice) 
	: MaterialProxie(RenderDevice.GetObjectsFactory().CreateMaterial(sizeof(MaterialProperties)))
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

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

	// Material
	//SetShader(EShaderType::VertexShader, g_pVertexShader);
	//SetShader(EShaderType::PixelShader, g_pPixelShader);
}

UI_Texture_Material::~UI_Texture_Material()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

void UI_Texture_Material::SetTexture(const std::shared_ptr<ITexture> _texture)
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
