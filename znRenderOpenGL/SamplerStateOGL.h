#pragma once

class SamplerStateOGL : public SamplerStateBase
{
public:
	                                                SamplerStateOGL();
	virtual                                         ~SamplerStateOGL();

    // SamplerState
	void                                            Bind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const override final;
	void                                            UnBind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const override final;

private:
	uint32_t                                        m_GLObj;
};