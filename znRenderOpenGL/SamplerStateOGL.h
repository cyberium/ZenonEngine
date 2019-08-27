#pragma once

class SamplerStateOGL : public SamplerState
{
public:
	                                                SamplerStateOGL();
	virtual                                         ~SamplerStateOGL();

    // SamplerState
	void                                            Bind(uint32_t ID, const Shader* shader, ShaderParameter::Type parameterType) const override final;
	void                                            UnBind(uint32_t ID, const Shader* shader, ShaderParameter::Type parameterType) const override final;

private:
	uint32_t                                        m_GLObj;
};