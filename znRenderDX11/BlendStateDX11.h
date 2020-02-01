#pragma once

class ZN_API BlendStateDX11 
	: public BlendStateBase
{
public:
	BlendStateDX11(IRenderDeviceDX11& RenderDeviceDX11);
	BlendStateDX11(const BlendStateDX11& copy);
	virtual	~BlendStateDX11();

	const BlendStateDX11&                           operator=(const BlendStateDX11& other);

    // BlendState
	void                                            Bind() override final;
	void                                            Unbind() override final;

private:
	ATL::CComPtr<ID3D11BlendState1>                 m_BlendState;

private: // Link to parent d3d11 device
	IRenderDeviceDX11&								m_RenderDeviceDX11;
};