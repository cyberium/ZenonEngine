#pragma once

class ZN_API ShaderParameterBase 
	: public IShaderParameter
{
public:
	ShaderParameterBase();
	ShaderParameterBase(const std::string& name, UINT slotID, IShader* shader, EType parameterType);
    virtual ~ShaderParameterBase();

	// IShaderParameter
	EType GetType() const override;
	void SetSource(std::shared_ptr<IShaderParameterSource> ShaderParameterSource) override final;
	std::shared_ptr<IShaderParameterSource> GetSource() const override final;
	void SetConstantBuffer(std::shared_ptr<IConstantBuffer> constantBuffer) override final;
	const std::shared_ptr<IConstantBuffer>& GetConstantBuffer() const override final;
	void SetTexture(std::shared_ptr<ITexture> texture) override final;
	const std::shared_ptr<ITexture>& GetTexture() const override final;
	void SetSampler(std::shared_ptr<ISamplerState> sampler) override final;
	const std::shared_ptr<ISamplerState>& GetSampler() const override final;
	void SetStructuredBuffer(std::shared_ptr<IStructuredBuffer> rwBuffer) override final;
	const std::shared_ptr<IStructuredBuffer>& GetStructuredBuffer() const override final;

	void Bind() override;
	void Unbind() override;

private:
	std::string             m_Name;
	UINT                    m_uiSlotID;
	const IShader*          m_Shader;
	EType                   m_ParameterType;

	std::shared_ptr<IConstantBuffer>  m_pConstantBuffer;
	std::shared_ptr<ITexture>         m_pTexture;
	std::shared_ptr<ISamplerState>    m_pSamplerState;
	std::shared_ptr<IStructuredBuffer> m_pStructuredBuffer;
};
