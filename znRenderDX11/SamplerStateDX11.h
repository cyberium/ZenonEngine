#pragma once

class OW_ENGINE_API SamplerStateDX11 : public SamplerState
{
public:
	                                                SamplerStateDX11(ID3D11Device2* pDevice);
	virtual                                         ~SamplerStateDX11();

    // SamplerState
	void                                            Bind(uint32_t ID, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType) override final;
	void                                            UnBind(uint32_t ID, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType) override final;

private:
	ATL::CComPtr<ID3D11Device2>                     m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2>              m_pDeviceContext;
	ATL::CComPtr<ID3D11SamplerState>                m_pSamplerState;
};