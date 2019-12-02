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


	std::shared_ptr<IShader> g_pVertexShader;
	std::shared_ptr<IShader> g_pPixelShader;

	if (_RenderDevice->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
	{
		g_pVertexShader = _RenderDevice->CreateShader(
			IShader::ShaderType::VertexShader, "IDB_SHADER_3D_TEXTURED", IShader::ShaderMacros(), "VS_main", "latest"
		);
		g_pPixelShader = _RenderDevice->CreateShader(
			IShader::ShaderType::PixelShader, "IDB_SHADER_3D_TEXTURED", IShader::ShaderMacros(), "PS_main", "latest"
		);
	}
	else
	{
		g_pVertexShader = _RenderDevice->CreateShader(
			IShader::ShaderType::VertexShader, "IDB_SHADER_OGL_3D_TEXTURED_VS", IShader::ShaderMacros(), "", ""
		);
		g_pPixelShader = _RenderDevice->CreateShader(
			IShader::ShaderType::PixelShader, "IDB_SHADER_OGL_3D_TEXTURED_PS", IShader::ShaderMacros(), "", ""
		);
	}
	//g_pVertexShader->LoadInputLayoutFromReflector();

	std::vector<SCustomVertexElement> elements;
	elements.push_back({ 0, 0,  ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION, 0 });
	elements.push_back({ 0, 12, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD, 0 });
	elements.push_back({ 0, 20, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::NORMAL, 0 });
	g_pVertexShader->LoadInputLayoutFromCustomElements(elements);

	std::shared_ptr<ISamplerState> g_Sampler = _RenderDevice->CreateSamplerState();
	g_Sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	g_Sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);

	// Assign samplers
	//g_pPixelShader->GetShaderParameterByName("DiffuseTextureSampler")->Set(g_Sampler.get());

	SetSampler(0, g_Sampler);

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

cvec4 MaterialTextured::GetDiffuseColor() const
{
	return m_pProperties->m_DiffuseColor;
}

//-----

void MaterialTextured::SetDiffuseColor(cvec4 diffuse)
{
	m_pProperties->m_DiffuseColor = diffuse;
	MarkConstantBufferDirty();
}

void MaterialTextured::UpdateConstantBuffer() const
{
	MaterialWrapper::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}