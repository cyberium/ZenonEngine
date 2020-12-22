#pragma once

// FORWARD BEGIN
ZN_INTERFACE IShader;
ZN_INTERFACE IBlendState;
ZN_INTERFACE IRasterizerState;
ZN_INTERFACE IDepthStencilState;
ZN_INTERFACE IRenderTarget;
// FORWARD END

ZN_INTERFACE ZN_API IPipelineState
{
	virtual ~IPipelineState() {}

	virtual void SetShader(std::shared_ptr<IShader> Shader) = 0;
	virtual const std::shared_ptr<IShader>& GetShader(EShaderType type) const = 0;
	virtual const ShaderMap& GetShaders() const = 0;
	virtual const IShader* GetVertexShaderPtr() const = 0;
	virtual const IShader* GetPixelShaderPtr() const = 0;

	virtual void SetTexture(uint8 ID, std::shared_ptr<ITexture> texture) = 0;
	virtual const std::shared_ptr<ITexture>& GetTexture(uint8 ID) const = 0;
	virtual const TextureMap& GetTextures() const = 0;

	virtual void SetSampler(uint8 ID, std::shared_ptr<ISamplerState> samplerState) = 0;
	virtual const std::shared_ptr<ISamplerState>& GetSampler(uint8 ID) const = 0;
	virtual const SamplersMap& GetSamplers() const = 0;

	virtual void SetBlendState(std::shared_ptr<IBlendState> blendState) = 0;
	virtual const std::shared_ptr<IBlendState>& GetBlendState() const = 0;

	virtual void SetRasterizerState(std::shared_ptr<IRasterizerState> rasterizerState) = 0;
	virtual const std::shared_ptr<IRasterizerState>& GetRasterizerState() const = 0;

	virtual void SetDepthStencilState(std::shared_ptr<IDepthStencilState> depthStencilState) = 0;
	virtual const std::shared_ptr<IDepthStencilState>& GetDepthStencilState() const  = 0;

	virtual void SetRenderTarget(std::shared_ptr<IRenderTarget> renderTarget) = 0;
	virtual const std::shared_ptr<IRenderTarget>& GetRenderTarget() const = 0;

	virtual void Bind() = 0;
	virtual void UnBind() = 0;
};

typedef std::vector< IPipelineState*> PipelineList;
