#pragma once

// FORWARD BEGIN
ZN_INTERFACE IShader;
ZN_INTERFACE IBlendState;
ZN_INTERFACE IRasterizerState;
ZN_INTERFACE IDepthStencilState;
ZN_INTERFACE IRenderTarget;
// FORWARD END

/**
 * The PipelineState class encompasses pipeline configuration.
 * This includes, shaders, blend states, rasterizer states,
 * and depth/stencil states among other things....
 */
ZN_INTERFACE ZN_API IPipelineState
{
	virtual ~IPipelineState() {}

	virtual void SetShader(IShader::ShaderType type, std::shared_ptr<IShader> pShader) = 0;
	virtual std::shared_ptr<IShader> GetShader(IShader::ShaderType type) const = 0;
	virtual const ShaderMap& GetShaders() const = 0;

	virtual void SetBlendState(const std::shared_ptr<IBlendState> blendState) = 0;
	virtual std::shared_ptr<IBlendState> GetBlendState() const = 0;

	virtual void SetRasterizerState(const std::shared_ptr<IRasterizerState> rasterizerState) = 0;
	virtual std::shared_ptr<IRasterizerState> GetRasterizerState() const = 0;

	virtual void SetDepthStencilState(const std::shared_ptr<IDepthStencilState> depthStencilState) = 0;
	virtual std::shared_ptr<IDepthStencilState> GetDepthStencilState() const  = 0;

	virtual void SetRenderTarget(std::shared_ptr<IRenderTarget> renderTarget) = 0;
	virtual std::shared_ptr<IRenderTarget> GetRenderTarget() const = 0;

	virtual void Bind() = 0;
	virtual void UnBind() = 0;
};

typedef std::vector< std::shared_ptr<IPipelineState>> PipelineList;
