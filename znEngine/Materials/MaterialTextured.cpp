#include "stdafx.h"

// General
#include "MaterialTextured.h"

// Additional
#include <Application.h>

MaterialTextured::MaterialTextured()
	: MaterialWrapper(_RenderDevice->CreateMaterial(sizeof(MaterialProperties)))
{
	// Constant buffer
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	std::shared_ptr<IShader> g_pVertexShader = _RenderDevice->CreateShader(
		IShader::ShaderType::VertexShader, "shaders_D3D/Debug/Textured.hlsl", IShader::ShaderMacros(), "VS_main", "latest"
	);
	std::vector<SCustomVertexElement> elements;
	elements.push_back({ 0, 0,  ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION, 0 });
	elements.push_back({ 0, 12, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD, 0 });
	elements.push_back({ 0, 20, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::NORMAL, 0 });
	g_pVertexShader->LoadInputLayoutFromCustomElements(elements);

	std::shared_ptr<IShader> g_pPixelShader = _RenderDevice->CreateShader(
		IShader::ShaderType::PixelShader, "shaders_D3D/Debug/Textured.hlsl", IShader::ShaderMacros(), "PS_main", "latest"
	);

	std::shared_ptr<ISamplerState> g_Sampler = _RenderDevice->CreateSamplerState();
	g_Sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	g_Sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);

	// Assign samplers
	g_pPixelShader->GetShaderParameterByName("DiffuseTextureSampler")->Set(g_Sampler.get());

	SetShader(IShader::ShaderType::VertexShader, g_pVertexShader);
	SetShader(IShader::ShaderType::PixelShader, g_pPixelShader);
}

MaterialTextured::~MaterialTextured()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

void MaterialTextured::SetTexture(std::shared_ptr<ITexture> texture)
{
	MaterialWrapper::SetTexture(0, texture);
}

void MaterialTextured::SetNormalTexture(std::shared_ptr<ITexture> texture)
{
	if (texture)
	{
		m_pProperties->m_HasNormalTexture = 1;
		MarkConstantBufferDirty();
		MaterialWrapper::SetTexture(1, texture);
	}
}

void MaterialTextured::SetHeightTexture(std::shared_ptr<ITexture> texture)
{
	if (texture)
	{
		m_pProperties->m_HasHeightTexture = 1;
		MarkConstantBufferDirty();
		MaterialWrapper::SetTexture(2, texture);
	}
}

void MaterialTextured::UpdateConstantBuffer() const
{
	MaterialWrapper::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}