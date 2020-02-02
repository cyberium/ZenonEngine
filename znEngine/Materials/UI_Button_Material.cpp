#include "stdafx.h"

// General
#include "UI_Button_Material.h"

UI_Button_Material::UI_Button_Material(IRenderDevice& RenderDevice) :
	MaterialProxie(RenderDevice.GetObjectsFactory().CreateMaterial(sizeof(MaterialProperties)))
{
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	// CreateShaders
	auto vertexShader = RenderDevice.GetObjectsFactory().CreateShader(EShaderType::VertexShader, "IDB_SHADER_UI_BUTTON", "VS_main");
	vertexShader->LoadInputLayoutFromReflector();

	auto pixelShader = RenderDevice.GetObjectsFactory().CreateShader(EShaderType::PixelShader, "IDB_SHADER_UI_BUTTON", "PS_main");

	// Create samplers
	auto g_LinearClampSampler = RenderDevice.GetObjectsFactory().CreateSamplerState();
	g_LinearClampSampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	g_LinearClampSampler->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);

	pixelShader->GetShaderParameterByName("DiffuseTextureSampler").Set(g_LinearClampSampler);

	// Material
	SetShader(EShaderType::VertexShader, vertexShader);
	SetShader(EShaderType::PixelShader, pixelShader);
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
