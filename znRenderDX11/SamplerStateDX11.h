#pragma once

class ZN_API SamplerStateDX11 : public SamplerStateBase
{
public:
	SamplerStateDX11(IRenderDeviceDX11* RenderDeviceD3D11);
	virtual	~SamplerStateDX11();

    // SamplerState
	void Bind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const override final;
	void UnBind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const override final;

private:
	mutable ATL::CComPtr<ID3D11SamplerState> m_pSamplerState;

private: // Link to parent d3d11 device
	IRenderDeviceDX11* m_RenderDeviceD3D11;
};