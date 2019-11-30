#pragma once

class OW_ENGINE_API ShaderParameterBase : public IShaderParameter, public Object
{
public:
	ShaderParameterBase();
	ShaderParameterBase(const std::string& name, UINT slotID, std::shared_ptr<IShader> shader, Type parameterType);
    virtual ~ShaderParameterBase();

	// IShaderParameter
	Type GetType() const;
	bool IsValid() const;

	void SetConstantBuffer(const IConstantBuffer* constantBuffer) override final;
	const IConstantBuffer* GetConstantBuffer() const override final;
	void SetTexture(const ITexture* texture) override final;
	const ITexture* GetTexture() const override final;
	void SetSampler(const ISamplerState* sampler) override final;
	const ISamplerState* GetSampler() const override final;
	void SetStructuredBuffer(const IStructuredBuffer* rwBuffer) override final;
	const IStructuredBuffer* GetStructuredBuffer() const override final;

	void Bind() override;
	void Unbind() override;

private:
	std::string             m_Name;
	UINT                    m_uiSlotID;
	const IShader*          m_Shader;
	Type                    m_ParameterType;

	const IConstantBuffer*  m_pConstantBuffer;
	const ITexture*         m_pTexture;
	const ISamplerState*    m_pSamplerState;
	const IStructuredBuffer*m_pStructuredBuffer;
};
