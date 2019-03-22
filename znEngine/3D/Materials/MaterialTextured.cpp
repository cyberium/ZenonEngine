#include "stdafx.h"

// Include
#include "MaterialWrapper.h"

// General
#include "MaterialTextured.h"

// Additional
#include <Application.h>

MaterialTextured::MaterialTextured(std::shared_ptr<Material> _material)
	: MaterialWrapper(_material)
{
	// Constant buffer
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	CreateConstantBuffer(m_pProperties, sizeof(MaterialProperties));


	std::shared_ptr<Shader> g_pVertexShader = _RenderDevice->CreateShader(
		Shader::VertexShader, "shaders_D3D/Debug/Textured.hlsl", Shader::ShaderMacros(), "VS_main", "latest"
	);
	std::vector<D3DVERTEXELEMENT9> elements;
	elements.push_back({ 0, 0,  D3DDECLTYPE_FLOAT3, 0, D3DDECLUSAGE_POSITION, 0 });
	elements.push_back({ 0, 12, D3DDECLTYPE_FLOAT2, 0, D3DDECLUSAGE_TEXCOORD, 0 });
	elements.push_back({ 0, 20, D3DDECLTYPE_FLOAT3, 0, D3DDECLUSAGE_NORMAL, 0 });
	g_pVertexShader->LoadInputLayoutFromD3DElement(elements);

	std::shared_ptr<Shader> g_pPixelShader = _RenderDevice->CreateShader(
		Shader::PixelShader, "shaders_D3D/Debug/Textured.hlsl", Shader::ShaderMacros(), "PS_main", "latest"
	);

	std::shared_ptr<SamplerState> g_Sampler = _RenderDevice->CreateSamplerState();
	g_Sampler->SetFilter(SamplerState::MinFilter::MinLinear, SamplerState::MagFilter::MagLinear, SamplerState::MipFilter::MipLinear);
	g_Sampler->SetWrapMode(SamplerState::WrapMode::Repeat, SamplerState::WrapMode::Repeat);

	// Assign samplers
	g_pPixelShader->GetShaderParameterByName("DiffuseTextureSampler").Set(g_Sampler);

	SetShader(Shader::ShaderType::VertexShader, g_pVertexShader);
	SetShader(Shader::ShaderType::PixelShader, g_pPixelShader);
}

MaterialTextured::~MaterialTextured()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

void MaterialTextured::SetTexture(std::shared_ptr<Texture> texture)
{
	MaterialWrapper::SetTexture(0, texture);
}

void MaterialTextured::SetNormalTexture(std::shared_ptr<Texture> texture)
{
	if (texture)
	{
		m_pProperties->m_HasNormalTexture = 1;
		MarkConstantBufferDirty();
		MaterialWrapper::SetTexture(1, texture);
	}
}

void MaterialTextured::SetHeightTexture(std::shared_ptr<Texture> texture)
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