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

MaterialOGL::MaterialOGL(IRenderDevice* renderDevice)
	: MaterialImpl(renderDevice)
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

void MaterialOGL::Bind() const
{
	MaterialImpl::Bind();

	for (auto shader : m_Shaders)
	{
		std::shared_ptr<ShaderOGL> pShader = std::dynamic_pointer_cast<ShaderOGL>(shader.second);
		if (pShader)
		{
			pShader->Bind();
			glUseProgramStages(m_GLProgramPipeline, GLTranslateShaderBitType(shader.first), pShader->GetGLObject());
		}
	}


	glBindProgramPipeline(m_GLProgramPipeline);
}

void MaterialOGL::Unbind() const
{
	glBindProgramPipeline(0);

	for (auto shader : m_Shaders)
	{
		std::shared_ptr<Shader> pShader = std::dynamic_pointer_cast<ShaderOGL>(shader.second);
		if (pShader)
		{
			glUseProgramStages(m_GLProgramPipeline, GLTranslateShaderBitType(shader.first), 0);
			pShader->UnBind();
		}
	}

	MaterialImpl::Unbind();
}
