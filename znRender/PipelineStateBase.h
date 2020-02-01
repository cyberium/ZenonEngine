#pragma once

class ZN_API PipelineStateBase
	: public IPipelineState
	, public Object
{
public:
	PipelineStateBase();
	virtual ~PipelineStateBase();

	// IPipelineState
	void SetShader(EShaderType type, const std::shared_ptr<IShader> pShader) override;
	const IShader& GetShader(EShaderType type) const override;
	const ShaderMap& GetShaders() const override;

	void SetTexture(uint8 ID, const std::shared_ptr<ITexture> texture) override;
	const ITexture& GetTexture(uint8 ID) const override;
	const TextureMap& GetTextures() const override;

	void SetSampler(uint8 ID, const std::shared_ptr<ISamplerState> samplerState) override;
	const ISamplerState& GetSampler(uint8 ID) const override;
	const SamplersMap& GetSamplers() const override;

	void SetBlendState(const std::shared_ptr<IBlendState> blendState) override;
	IBlendState& GetBlendState() const override;

	void SetRasterizerState(const std::shared_ptr<IRasterizerState> rasterizerState) override;
	IRasterizerState& GetRasterizerState() const override;

	void SetDepthStencilState(const std::shared_ptr<IDepthStencilState> depthStencilState) override;
	IDepthStencilState& GetDepthStencilState() const override;

	void SetRenderTarget(const std::shared_ptr<IRenderTarget> renderTarget) override;
	const IRenderTarget& GetRenderTarget() const override;

protected:
	ShaderMap                                       m_Shaders;
	TextureMap										m_Textures;
	SamplersMap										m_Samplers;

	std::shared_ptr<IBlendState>                    m_BlendState;
	std::shared_ptr<IRasterizerState>				m_RasterizerState;
	std::shared_ptr<IDepthStencilState>				m_DepthStencilState;
	std::shared_ptr<IRenderTarget>                  m_RenderTarget;
};