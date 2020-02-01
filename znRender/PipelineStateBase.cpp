#include "stdafx.h"

// General
#include "PipelineStateBase.h"


PipelineStateBase::PipelineStateBase()
{
}

PipelineStateBase::~PipelineStateBase()
{
}



//
// IPipelineState
//
void PipelineStateBase::SetShader(EShaderType type, const std::shared_ptr<IShader> pShader)
{
	m_Shaders[type] = pShader;
}

const IShader& PipelineStateBase::GetShader(EShaderType type) const
{
	const auto& iter = m_Shaders.find(type);
	if (iter == m_Shaders.end())
		throw CException(L"Shader not found.");

	return *iter->second;
}

const ShaderMap& PipelineStateBase::GetShaders() const
{
	return m_Shaders;
}

void PipelineStateBase::SetTexture(uint8 ID, const std::shared_ptr < ITexture> texture)
{
	m_Textures[ID] = texture;
}

const ITexture& PipelineStateBase::GetTexture(uint8 ID) const
{
	const auto& itr = m_Textures.find(ID);
	if (itr == m_Textures.end())
		throw CException(L"Texture not found.");

	return *itr->second;
}

const TextureMap& PipelineStateBase::GetTextures() const
{
	return m_Textures;
}

void PipelineStateBase::SetSampler(uint8 ID, const std::shared_ptr<ISamplerState> samplerState)
{
	m_Samplers[ID] = samplerState;
}

const ISamplerState& PipelineStateBase::GetSampler(uint8 ID) const
{
	const auto& itr = m_Samplers.find(ID);
	if (itr == m_Samplers.end())
		throw CException(L"Sampler not found.");

	return *itr->second;
}

const SamplersMap& PipelineStateBase::GetSamplers() const
{
	return m_Samplers;
}

void PipelineStateBase::SetBlendState(const std::shared_ptr<IBlendState> blendState)
{
	m_BlendState = blendState;
}

IBlendState& PipelineStateBase::GetBlendState() const
{
	return *m_BlendState;
}

void PipelineStateBase::SetRasterizerState(const std::shared_ptr<IRasterizerState > rasterizerState)
{
	m_RasterizerState = rasterizerState;
}

IRasterizerState& PipelineStateBase::GetRasterizerState() const
{
	return *m_RasterizerState;
}

void PipelineStateBase::SetDepthStencilState(const std::shared_ptr<IDepthStencilState > depthStencilState)
{
	m_DepthStencilState = depthStencilState;
}

IDepthStencilState& PipelineStateBase::GetDepthStencilState() const
{
	return *m_DepthStencilState;
}

void PipelineStateBase::SetRenderTarget(const std::shared_ptr<IRenderTarget> renderTarget)
{
	m_RenderTarget = renderTarget;
}

const IRenderTarget& PipelineStateBase::GetRenderTarget() const
{
	return *m_RenderTarget;
}
