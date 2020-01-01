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

void PipelineStateOGL::SetShader(IShader::ShaderType type, std::shared_ptr<IShader> pShader)
{
	m_Shaders[type] = pShader;

	if (pShader)
	{
		std::shared_ptr<ShaderOGL> shaderOGL = std::dynamic_pointer_cast<ShaderOGL>(pShader);
		glUseProgramStages(m_GLProgramPipeline, GLTranslateShaderBitType(type), shaderOGL->GetGLObject());
	}
}

std::shared_ptr<IShader> PipelineStateOGL::GetShader(IShader::ShaderType type) const
{
	ShaderMap::const_iterator iter = m_Shaders.find(type);
	if (iter != m_Shaders.end())
	{
		return iter->second;
	}

	return nullptr;
}

const ShaderMap & PipelineStateOGL::GetShaders() const
{
	return m_Shaders;
}

void PipelineStateOGL::SetBlendState(const std::shared_ptr<IBlendState> blendState)
{
	m_BlendState = std::dynamic_pointer_cast<BlendStateOGL>(blendState);
}

std::shared_ptr<IBlendState> PipelineStateOGL::GetBlendState() const
{
	return m_BlendState;
}

void PipelineStateOGL::SetRasterizerState(const std::shared_ptr<IRasterizerState> rasterizerState)
{
	m_RasterizerState = std::dynamic_pointer_cast<RasterizerStateOGL>(rasterizerState);
}

std::shared_ptr<IRasterizerState> PipelineStateOGL::GetRasterizerState() const
{
	return m_RasterizerState;
}

void PipelineStateOGL::SetDepthStencilState(const std::shared_ptr<IDepthStencilState> depthStencilState)
{
	m_DepthStencilState = std::dynamic_pointer_cast<DepthStencilStateOGL>(depthStencilState);
}

std::shared_ptr<IDepthStencilState> PipelineStateOGL::GetDepthStencilState() const
{
	return m_DepthStencilState;
}

void PipelineStateOGL::SetRenderTarget(std::shared_ptr<IRenderTarget> renderTarget)
{
	m_RenderTarget = renderTarget;
}

std::shared_ptr<IRenderTarget> PipelineStateOGL::GetRenderTarget() const
{
	return m_RenderTarget;
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
}

void PipelineStateOGL::UnBind()
{
	glBindProgramPipeline(0);

	if (m_RenderTarget)
	{
		m_RenderTarget->UnBind();
	}
}