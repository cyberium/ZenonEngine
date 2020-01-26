#pragma once

#include "TextureDX11.h"
#include "RenderTargetDX11.h"

class ZN_API RenderWindowDX11 
	: public RenderWindowBase
{
public:
	RenderWindowDX11(IRenderDeviceDX11* RenderDeviceD3D11, INativeWindow * WindowObject, bool vSync);
	virtual ~RenderWindowDX11();

	void                                            Present() override final;

protected:
    void                                            CreateSwapChain() override final;
	void                                            ResizeSwapChainBuffers(uint32_t width, uint32_t height) override final;

private:
	ATL::CComPtr<IDXGISwapChain2>                   m_pSwapChain;
	ATL::CComPtr<ID3D11Texture2D>                   m_pBackBuffer;

	DXGI_SAMPLE_DESC m_SampleDesc;      // Used to enable multisampling AA

private: // Link to parent d3d11 device
	IRenderDeviceDX11* m_RenderDeviceD3D11;
};