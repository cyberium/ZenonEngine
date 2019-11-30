#include "stdafx.h"

// General
#include "MaterialOGL.h"

// Additional
#include "ShaderOGL.h"

GLbitfield GLTranslateShaderBitType(Shader::ShaderType _type)
{
	GLbitfield result = GL_VERTEX_SHADER_BIT;
	switch (_type)
	{
	case Shader::ShaderType::VertexShader:
		result = GL_VERTEX_SHADER_BIT;
		break;
	case Shader::ShaderType::PixelShader:
		result = GL_FRAGMENT_SHADER_BIT;
		break;
	case Shader::ShaderType::GeometryShader:
		result = GL_GEOMETRY_SHADER_BIT;
		break;
	case Shader::ShaderType::TessellationControlShader:
		result = GL_TESS_CONTROL_SHADER_BIT;
		break;
	case Shader::ShaderType::TessellationEvaluationShader:
		result = GL_TESS_EVALUATION_SHADER_BIT;
		break;
	case Shader::ShaderType::ComputeShader:
		result = GL_COMPUTE_SHADER_BIT;
		break;
	default:
		Log::Error("Unknown shader type factor.");
	}

	return result;
}

MaterialOGL::MaterialOGL(IRenderDevice* renderDevice, size_t Size)
	: MaterialBase(renderDevice, Size)
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

void MaterialOGL::SetShader(Shader::ShaderType type, std::shared_ptr<Shader> pShader)
{
    MaterialBase::SetShader(type, pShader);

    std::shared_ptr<ShaderOGL> shaderOGL = std::dynamic_pointer_cast<ShaderOGL>(pShader);
    glUseProgramStages(m_GLProgramPipeline, GLTranslateShaderBitType(type), shaderOGL->GetGLObject());
}

void MaterialOGL::Bind() const
{
	MaterialBase::Bind();

	for (auto shader : m_Shaders)
	{
		std::shared_ptr<ShaderOGL> pShader = std::dynamic_pointer_cast<ShaderOGL>(shader.second);
		if (pShader)
		{
			pShader->Bind();

            for (auto textureIt : m_Textures)
            {
                std::shared_ptr<ITexture> texture = textureIt.second;
                texture->Bind((uint32_t)textureIt.first, pShader.get(), ShaderParameter::Type::Texture);
            }

            for (auto samplerStateIt : m_Samplers)
            {
                std::shared_ptr<ISamplerState> samplerState = samplerStateIt.second;
                samplerState->Bind((uint32_t)samplerStateIt.first, pShader.get(), ShaderParameter::Type::Sampler);
            }

            ShaderParameter& materialParameter = pShader->GetShaderParameterByName("Material");
            if (materialParameter.IsValid() && m_pConstantBuffer != nullptr)
            {
                materialParameter.Set<ConstantBuffer>(m_pConstantBuffer.get());
                materialParameter.Bind();
            }
		}
	}

	glBindProgramPipeline(m_GLProgramPipeline);
}

void MaterialOGL::Unbind() const
{
	for (auto shader : m_Shaders)
	{
		std::shared_ptr<Shader> pShader = std::dynamic_pointer_cast<ShaderOGL>(shader.second);
		if (pShader)
		{
            for (auto textureIt : m_Textures)
            {
                std::shared_ptr<ITexture> texture = textureIt.second;
                texture->UnBind((uint32_t)textureIt.first, pShader.get(), ShaderParameter::Type::Texture);
            }

            for (auto samplerStateIt : m_Samplers)
            {
                std::shared_ptr<ISamplerState> samplerState = samplerStateIt.second;
                samplerState->UnBind((uint32_t)samplerStateIt.first, pShader.get(), ShaderParameter::Type::Sampler);
            }

            ShaderParameter& materialParameter = pShader->GetShaderParameterByName("Material");
            if (materialParameter.IsValid() && m_pConstantBuffer != nullptr)
            {
                materialParameter.UnBind();
            }

			glUseProgramStages(m_GLProgramPipeline, GLTranslateShaderBitType(shader.first), 0);
			pShader->UnBind();
		}
	}

    glBindProgramPipeline(0);

	MaterialBase::Unbind();
}
