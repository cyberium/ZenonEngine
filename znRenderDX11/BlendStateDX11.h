#pragma once

class ZN_API BlendStateDX11 
	: public BlendStateBase
{
public:
	BlendStateDX11(IRenderDeviceDX11* RenderDeviceD3D11);
	BlendStateDX11(const BlendStateDX11& copy);
	virtual	~BlendStateDX11();

	const BlendStateDX11&                           operator=(const BlendStateDX11& other);

    // BlendState
	void                                            Bind() override final;

private:
	ATL::CComPtr<ID3D11BlendState1>                 m_pBlendState;

private: // Link to parent d3d11 device
	IRenderDeviceDX11* m_RenderDeviceD3D11;
};