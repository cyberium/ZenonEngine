#pragma once

#include "BlendStateOGL.h"
#include "RasterizerStateOGL.h"
#include "DepthStencilStateOGL.h"

class PipelineStateOGL : public IPipelineState
{
public:
	PipelineStateOGL();
	virtual ~PipelineStateOGL();

	void SetShader(IShader::ShaderType type, std::shared_ptr<IShader> pShader);
	std::shared_ptr<IShader> GetShader(IShader::ShaderType type) const;
	const ShaderMap& GetShaders() const override;

	void SetBlendState(const std::shared_ptr<IBlendState> blendState);
	std::shared_ptr<IBlendState> GetBlendState() const;

	void SetRasterizerState(const std::shared_ptr<IRasterizerState> rasterizerState);
	std::shared_ptr<IRasterizerState> GetRasterizerState() const;

	void SetDepthStencilState(const std::shared_ptr<IDepthStencilState> depthStencilState);
	std::shared_ptr<IDepthStencilState> GetDepthStencilState() const;

	void SetRenderTarget(std::shared_ptr<IRenderTarget> renderTarget);
	std::shared_ptr<IRenderTarget> GetRenderTarget() const;

	void Bind();
	void UnBind();

private:
	ShaderMap m_Shaders;
	std::shared_ptr<BlendStateOGL> m_BlendState;
	std::shared_ptr<RasterizerStateOGL> m_RasterizerState;
	std::shared_ptr<DepthStencilStateOGL> m_DepthStencilState;
	std::shared_ptr<IRenderTarget> m_RenderTarget;

	uint32 m_GLProgramPipeline;
};