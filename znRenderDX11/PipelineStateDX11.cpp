#include "stdafx.h"

#include "PipelineStateDX11.h"

PipelineStateDX11::PipelineStateDX11(IRenderDeviceDX11* RenderDeviceD3D11)
	: m_RenderDeviceD3D11(RenderDeviceD3D11)
{
	m_BlendState = std::make_shared<BlendStateDX11>(m_RenderDeviceD3D11);
	m_RasterizerState = std::make_shared<RasterizerStateDX11>(m_RenderDeviceD3D11);
	m_DepthStencilState = std::make_shared<DepthStencilStateDX11>(m_RenderDeviceD3D11);
}

PipelineStateDX11::~PipelineStateDX11()
{
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
		const IShader* shader = it.second;
		_ASSERT(shader != nullptr);

		shader->Bind();

		if (shader->GetType() == EShaderType::PixelShader)
		{
			for (const auto& textureIt : m_Textures)
			{
				const ITexture* texture = textureIt.second;
				_ASSERT(texture != nullptr);

				texture->Bind((uint32_t)textureIt.first, shader, IShaderParameter::Type::Texture);
			}

			for (const auto& samplerStateIt : m_Samplers)
			{
				const ISamplerState* samplerState = samplerStateIt.second;
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
		const IShader* shader = it.second;
		_ASSERT(shader != nullptr);

		if (shader->GetType() == EShaderType::PixelShader)
		{
			for (const auto& textureIt : m_Textures)
			{
				const ITexture* texture = textureIt.second;
				_ASSERT(texture != nullptr);

				texture->UnBind((uint32_t)textureIt.first, shader, IShaderParameter::Type::Texture);
			}

			for (const auto& samplerStateIt : m_Samplers)
			{
				const ISamplerState* samplerState = samplerStateIt.second;
				_ASSERT(samplerState != nullptr);

				samplerState->UnBind((uint32_t)samplerStateIt.first, shader, IShaderParameter::Type::Sampler);
			}
		}

		shader->UnBind();
	}

    if (m_RenderTarget)
    {
        m_RenderTarget->UnBind();
    }
}