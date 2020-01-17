#include "stdafx.h"

#include "PipelineStateDX11.h"

PipelineStateDX11::PipelineStateDX11(ID3D11Device2* pDevice)
	: m_pDevice(pDevice)
{
	m_pDevice->GetImmediateContext2(&m_pDeviceContext);

	m_BlendState = std::make_shared<BlendStateDX11>(pDevice);
	m_RasterizerState = std::make_shared<RasterizerStateDX11>(pDevice);
	m_DepthStencilState = std::make_shared<DepthStencilStateDX11>(pDevice);
}

PipelineStateDX11::~PipelineStateDX11()
{
}



//
// PipelineState
//
void PipelineStateDX11::SetShader(EShaderType type, std::shared_ptr<IShader> pShader)
{
	m_Shaders[type] = pShader;
}

std::shared_ptr<IShader> PipelineStateDX11::GetShader(EShaderType type) const
{
	ShaderMap::const_iterator iter = m_Shaders.find(type);
	if (iter != m_Shaders.end())
		return iter->second;

	return nullptr;
}

const ShaderMap& PipelineStateDX11::GetShaders() const
{
	return m_Shaders;
}

void PipelineStateDX11::SetTexture(uint8 ID, std::shared_ptr<ITexture> texture)
{
	m_Textures[ID] = texture;
}

std::shared_ptr<ITexture> PipelineStateDX11::GetTexture(uint8 ID) const
{
	const auto& itr = m_Textures.find(ID);
	if (itr == m_Textures.end())
	{
		_ASSERT_EXPR(false, L"Sampler not found.");
		return nullptr;
	}

	return itr->second;
}

const TextureMap& PipelineStateDX11::GetTextures() const
{
	return m_Textures;
}

void PipelineStateDX11::SetSampler(uint8 ID, std::shared_ptr<ISamplerState> samplerState)
{
	m_Samplers[ID] = samplerState;
}

std::shared_ptr<ISamplerState> PipelineStateDX11::GetSampler(uint8 ID) const
{
	const auto& itr = m_Samplers.find(ID);
	if (itr == m_Samplers.end())
	{
		_ASSERT_EXPR(false, L"Sampler not found.");
		return nullptr;
	}

	return itr->second;
}

const SamplersMap& PipelineStateDX11::GetSamplers() const
{
	return m_Samplers;
}



void PipelineStateDX11::SetBlendState(const std::shared_ptr<IBlendState> blendState)
{
	m_BlendState = std::dynamic_pointer_cast<BlendStateDX11>(blendState);
}

std::shared_ptr<IBlendState > PipelineStateDX11::GetBlendState() const
{
	return m_BlendState;
}

void PipelineStateDX11::SetRasterizerState(const std::shared_ptr<IRasterizerState > rasterizerState)
{
	m_RasterizerState = std::dynamic_pointer_cast<RasterizerStateDX11>(rasterizerState);
}

std::shared_ptr<IRasterizerState > PipelineStateDX11::GetRasterizerState() const
{
	return m_RasterizerState;
}

void PipelineStateDX11::SetDepthStencilState(const std::shared_ptr<IDepthStencilState > depthStencilState)
{
	m_DepthStencilState = std::dynamic_pointer_cast<DepthStencilStateDX11>(depthStencilState);
}

std::shared_ptr<IDepthStencilState > PipelineStateDX11::GetDepthStencilState() const
{
	return m_DepthStencilState;
}

void PipelineStateDX11::SetRenderTarget(std::shared_ptr<IRenderTarget> renderTarget)
{
	m_RenderTarget = renderTarget;
}

std::shared_ptr<IRenderTarget> PipelineStateDX11::GetRenderTarget() const
{
	return m_RenderTarget;
}

void PipelineStateDX11::Bind()
{
	if (m_RenderTarget)
	{
		m_RenderTarget->Bind();
	}

	m_BlendState->Bind();
	m_RasterizerState->Bind();
	m_DepthStencilState->Bind();

	for (const auto& it : m_Shaders)
	{
		const IShader* shader = it.second.get();
		_ASSERT(shader != nullptr);

		shader->Bind();

		if (it.second->GetType() == EShaderType::PixelShader)
		{
			for (const auto& textureIt : m_Textures)
			{
				const ITexture* texture = textureIt.second.get();
				_ASSERT(texture != nullptr);

				texture->Bind((uint32_t)textureIt.first, shader, IShaderParameter::Type::Texture);
			}

			for (const auto& samplerStateIt : m_Samplers)
			{
				const ISamplerState* samplerState = samplerStateIt.second.get();
				_ASSERT(samplerState != nullptr);

				samplerState->Bind((uint32_t)samplerStateIt.first, shader, IShaderParameter::Type::Sampler);
			}
		}
	}
}

void PipelineStateDX11::UnBind()
{
	for (const auto& it : m_Shaders)
	{
		const IShader* shader = it.second.get();
		_ASSERT(shader != nullptr);

		shader->UnBind();
	}

    if (m_RenderTarget)
    {
        m_RenderTarget->UnBind();
    }
}