#include "stdafx.h"

// General
#include "MaterialOGL.h"

// Additional
#include "ShaderOGL.h"

MaterialOGL::MaterialOGL(IRenderDevice* RenderDevice, size_t Size)
	: MaterialBase(RenderDevice, Size)
{
	glGenProgramPipelines(1, &m_GLProgramPipeline);
}

MaterialOGL::~MaterialOGL()
{
	if (m_GLProgramPipeline != 0)
	{
		glDeleteProgramPipelines(1, &m_GLProgramPipeline);
		m_GLProgramPipeline = 0;
	}
}

void MaterialOGL::SetShader(EShaderType type, IShader* pShader)
{
    MaterialBase::SetShader(type, pShader);

	if (pShader)
	{
		std::shared_ptr<ShaderOGL> shaderOGL = std::dynamic_pointer_cast<ShaderOGL>(pShader);
		glUseProgramStages(m_GLProgramPipeline, GLTranslateShaderBitType(type), shaderOGL->GetGLObject());
	}
}

void MaterialOGL::Bind(const ShaderMap& shaders) const
{
	MaterialBase::Bind(shaders);

	// means shader in Pipeline state
	if (m_Shaders.empty())
	{
		for (auto shader : shaders)
		{
			BindForShader(shader.second);
		}
	}
	else
	{
		for (auto shader : m_Shaders)
		{
			shader.second->Bind();

			BindForShader(shader.second);
		}

		glBindProgramPipeline(m_GLProgramPipeline);
	}
}

void MaterialOGL::Unbind(const ShaderMap& shaders) const
{
	// means shader in Pipeline state
	if (m_Shaders.empty())
	{
		for (auto shader : shaders)
		{
			UnbindForShader(shader.second);
		}
	}
	else
	{
		for (auto shader : m_Shaders)
		{
			//UnbindForShader(shader.second);
		}

		glBindProgramPipeline(0);
	}

	MaterialBase::Unbind(shaders);
}

void MaterialOGL::BindForShader(const IShader * shader) const
{
	const IShader* pShader = shader;
	if (pShader)
	{
		for (const auto& textureIt : m_Textures)
		{
			ITexture* texture = textureIt.second;
			texture->Bind((uint32_t)textureIt.first, pShader, IShaderParameter::Type::Texture);
		}

		for (const auto& samplerStateIt : m_Samplers)
		{
			ISamplerState* samplerState = samplerStateIt.second;
			samplerState->Bind((uint32_t)samplerStateIt.first, pShader, IShaderParameter::Type::Sampler);
		}

		const IShaderParameter*& materialParameter = pShader->GetShaderParameterByName("Material");
		if (materialParameter->IsValid() && m_pConstantBuffer != nullptr)
		{
			materialParameter->SetConstantBuffer(m_pConstantBuffer);
			materialParameter->Bind();
		}
	}
}

void MaterialOGL::UnbindForShader(const IShader * shader) const
{
	const IShader* pShader = shader;
	if (pShader)
	{
		for (const auto& textureIt : m_Textures)
		{
			ITexture* texture = textureIt.second;
			texture->UnBind((uint32_t)textureIt.first, pShader, IShaderParameter::Type::Texture);
		}

		for (const auto& samplerStateIt : m_Samplers)
		{
			ISamplerState* samplerState = samplerStateIt.second;
			samplerState->UnBind((uint32_t)samplerStateIt.first, pShader, IShaderParameter::Type::Sampler);
		}

		const IShaderParameter*& materialParameter = pShader->GetShaderParameterByName("Material");
		if (materialParameter->IsValid() && m_pConstantBuffer != nullptr)
		{
			materialParameter->Unbind();
		}
	}
}
