#include "stdafx.h"

#include "PipelineStateOGL.h"

// Additional
#include "ShaderOGL.h"

PipelineStateOGL::PipelineStateOGL()
{
	m_BlendState = std::make_shared<BlendStateOGL>();
	m_RasterizerState = std::make_shared<RasterizerStateOGL>();
	m_DepthStencilState = std::make_shared<DepthStencilStateOGL>();

	glGenProgramPipelines(1, &m_GLProgramPipeline);
}

PipelineStateOGL::~PipelineStateOGL()
{
	if (m_GLProgramPipeline != 0)
	{
		glDeleteProgramPipelines(1, &m_GLProgramPipeline);
		m_GLProgramPipeline = 0;
	}
}

void PipelineStateOGL::SetShader(EShaderType type, IShader* pShader)
{
	PipelineStateBase::SetShader(type, pShader);

	if (pShader)
	{
		std::shared_ptr<ShaderOGL> shaderOGL = std::dynamic_pointer_cast<ShaderOGL>(pShader);
		glUseProgramStages(m_GLProgramPipeline, GLTranslateShaderBitType(type), shaderOGL->GetGLObject());
	}
}

void PipelineStateOGL::Bind()
{
	if (m_RenderTarget)
	{
		m_RenderTarget->Bind();
	}

	m_BlendState->Bind();
	m_RasterizerState->Bind();
	m_DepthStencilState->Bind();

	glBindProgramPipeline(m_GLProgramPipeline);

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

void PipelineStateOGL::UnBind()
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

	glBindProgramPipeline(0);

	if (m_RenderTarget)
	{
		m_RenderTarget->UnBind();
	}
}