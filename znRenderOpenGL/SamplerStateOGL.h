#pragma once

class SamplerStateOGL : public SamplerState
{
public:
	                                                SamplerStateOGL();
	virtual                                         ~SamplerStateOGL();

    // SamplerState
	void                                            Bind(uint32_t ID, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType) override final;
	void                                            UnBind(uint32_t ID, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType) override final;

private:
	uint32_t                                        m_GLObj;
};