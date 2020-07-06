#include "stdafx.h"

#include "PipelineStateDX11.h"

PipelineStateDX11::PipelineStateDX11(IRenderDeviceDX11& RenderDeviceDX11)
	: m_RenderDeviceDX11(RenderDeviceDX11)
{
	m_BlendState = m_RenderDeviceDX11.GetObjectsFactory().CreateBlendState();
	m_RasterizerState = m_RenderDeviceDX11.GetObjectsFactory().CreateRasterizerState();
	m_DepthStencilState = m_RenderDeviceDX11.GetObjectsFactory().CreateDepthStencilState();
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
		const auto& shader = it.second;
		_ASSERT(shader != nullptr);

		shader->Bind();

		if (shader->GetShaderType() == EShaderType::PixelShader)
		{
			for (const auto& textureIt : m_Textures)
			{
				const auto& texture = textureIt.second;
				_ASSERT(texture != nullptr);

				texture->Bind((uint32_t)textureIt.first, shader.get(), IShaderParameter::Type::Texture);
			}

			for (const auto& samplerStateIt : m_Samplers)
			{
				const auto& samplerState = samplerStateIt.second;
				_ASSERT(samplerState != nullptr);

				samplerState->Bind((uint32_t)samplerStateIt.first, shader.get(), IShaderParameter::Type::Sampler);
			}
		}
	}
}

void PipelineStateDX11::UnBind()
{
	for (const auto& it : m_Shaders)
	{
		const auto& shader = it.second;
		_ASSERT(shader != nullptr);

		if (shader->GetShaderType() == EShaderType::PixelShader)
		{
			for (const auto& textureIt : m_Textures)
			{
				const auto& texture = textureIt.second;
				_ASSERT(texture != nullptr);
				texture->UnBind((uint32_t)textureIt.first, shader.get(), IShaderParameter::Type::Texture);
			}

			for (const auto& samplerStateIt : m_Samplers)
			{
				const auto& samplerState = samplerStateIt.second;
				_ASSERT(samplerState != nullptr);
				samplerState->UnBind((uint32_t)samplerStateIt.first, shader.get(), IShaderParameter::Type::Sampler);
			}
		}

		shader->UnBind();
	}

	m_DepthStencilState->Unbind();
	m_RasterizerState->Unbind();
	m_BlendState->Unbind();

    if (m_RenderTarget)
    {
        m_RenderTarget->UnBind();
    }
}