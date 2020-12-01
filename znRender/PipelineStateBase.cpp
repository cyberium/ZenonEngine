#include "stdafx.h"

// General
#include "PipelineStateBase.h"


PipelineStateBase::PipelineStateBase()
	: m_VertexShader(nullptr)
	, m_PixelShader(nullptr)
{
}

PipelineStateBase::~PipelineStateBase()
{
}



//
// IPipelineState
//
void PipelineStateBase::SetShader(EShaderType type, std::shared_ptr<IShader> Shader)
{
	m_Shaders[type] = Shader;

	if (type == EShaderType::VertexShader)
		m_VertexShader = Shader.get();
	else if (type == EShaderType::PixelShader)
		m_PixelShader = Shader.get();
}

const std::shared_ptr<IShader>& PipelineStateBase::GetShader(EShaderType type) const
{
	const auto& iter = m_Shaders.find(type);
	if (iter == m_Shaders.end())
		throw CException("Shader '%d' not found.", type);
	return iter->second;
}

const ShaderMap& PipelineStateBase::GetShaders() const
{
	return m_Shaders;
}

const IShader * PipelineStateBase::GetVertexShaderPtr() const
{
	return m_VertexShader;
}

const IShader * PipelineStateBase::GetPixelShaderPtr() const
{
	return m_PixelShader;
}

void PipelineStateBase::SetTexture(uint8 ID, const std::shared_ptr < ITexture> texture)
{
	m_Textures[ID] = texture;
}

const std::shared_ptr<ITexture>& PipelineStateBase::GetTexture(uint8 ID) const
{
	const auto& itr = m_Textures.find(ID);
	if (itr == m_Textures.end())
		throw CException(L"Texture with id '%d' not found.", ID);
	return itr->second;
}

const TextureMap& PipelineStateBase::GetTextures() const
{
	return m_Textures;
}

void PipelineStateBase::SetSampler(uint8 ID, const std::shared_ptr<ISamplerState> samplerState)
{
	m_Samplers[ID] = samplerState;
}

const std::shared_ptr<ISamplerState>& PipelineStateBase::GetSampler(uint8 ID) const
{
	const auto& itr = m_Samplers.find(ID);
	if (itr == m_Samplers.end())
		throw CException(L"Sampler with id not found.", ID);
	return itr->second;
}

const SamplersMap& PipelineStateBase::GetSamplers() const
{
	return m_Samplers;
}

void PipelineStateBase::SetBlendState(const std::shared_ptr<IBlendState> blendState)
{
	m_BlendState = blendState;
}

const std::shared_ptr<IBlendState>& PipelineStateBase::GetBlendState() const
{
	return m_BlendState;
}

void PipelineStateBase::SetRasterizerState(const std::shared_ptr<IRasterizerState > rasterizerState)
{
	m_RasterizerState = rasterizerState;
}

const std::shared_ptr<IRasterizerState>& PipelineStateBase::GetRasterizerState() const
{
	return m_RasterizerState;
}

void PipelineStateBase::SetDepthStencilState(const std::shared_ptr<IDepthStencilState > depthStencilState)
{
	m_DepthStencilState = depthStencilState;
}

const std::shared_ptr<IDepthStencilState>& PipelineStateBase::GetDepthStencilState() const
{
	return m_DepthStencilState;
}

void PipelineStateBase::SetRenderTarget(const std::shared_ptr<IRenderTarget> renderTarget)
{
	_ASSERT(renderTarget != nullptr);
	m_RenderTarget = renderTarget;
}

const std::shared_ptr<IRenderTarget>& PipelineStateBase::GetRenderTarget() const
{
	return m_RenderTarget;
}
