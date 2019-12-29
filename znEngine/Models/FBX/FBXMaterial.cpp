#include "stdafx.h"

// General
#include "FBXMaterial.h"

CFBXMaterial::CFBXMaterial(std::shared_ptr<IRenderDevice> RenderDevice)
	: MaterialProxie(RenderDevice->CreateMaterial(sizeof(MaterialProperties)))
	, m_RenderDevice(RenderDevice)
{
	// Constant buffer
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	std::shared_ptr<ISamplerState> g_Sampler = RenderDevice->CreateSamplerState();
	g_Sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	g_Sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	SetSampler(0, g_Sampler);
}

CFBXMaterial::~CFBXMaterial()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}
