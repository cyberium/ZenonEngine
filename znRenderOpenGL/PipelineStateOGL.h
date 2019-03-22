#pragma once

#include "BlendStateOGL.h"
#include "RasterizerStateOGL.h"
#include "DepthStencilStateOGL.h"

class PipelineStateOGL : public PipelineState
{
public:
	PipelineStateOGL();
	virtual ~PipelineStateOGL();

	virtual void SetShader(Shader::ShaderType type, std::shared_ptr<Shader> pShader);
	virtual std::shared_ptr<Shader> GetShader(Shader::ShaderType type) const;

	virtual void SetBlendState(const BlendState& blendState);
	virtual BlendState& GetBlendState();

	virtual void SetRasterizerState(const RasterizerState& rasterizerState);
	virtual RasterizerState& GetRasterizerState();

	virtual void SetDepthStencilState(const DepthStencilState& depthStencilState);
	virtual DepthStencilState& GetDepthStencilState();

	virtual void SetRenderTarget(std::shared_ptr<IRenderTarget> renderTarget);
	virtual std::shared_ptr<IRenderTarget> GetRenderTarget() const;

	virtual void Bind();
	virtual void UnBind();

private:
	ShaderMap m_Shaders;
	BlendStateOGL m_BlendState;
	RasterizerStateOGL m_RasterizerState;
	DepthStencilStateOGL m_DepthStencilState;
	std::shared_ptr<IRenderTarget> m_RenderTarget;
};