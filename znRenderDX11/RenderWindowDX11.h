#pragma once

#include "TextureDX11.h"
#include "RenderTargetDX11.h"

class RenderWindowDX11 : public RenderWindow
{
	typedef RenderWindow base;
public:
	RenderWindowDX11(std::shared_ptr<RenderDeviceDX11> device, IWindowObject * WindowObject, bool vSync);
	virtual ~RenderWindowDX11();

	virtual void Present();

	virtual std::shared_ptr<IRenderTarget> GetRenderTarget();

protected:
	virtual void CreateSwapChain();

	// Engine events
	virtual void OnPreRender(Render3DEventArgs& e);
	virtual void OnPostRender(Render3DEventArgs& e);

	// Window events
	virtual void OnResize(ResizeEventArgs& e);

protected:
	virtual void ResizeSwapChainBuffers(uint32_t width, uint32_t height);

private:
	std::weak_ptr<RenderDeviceDX11>   m_Device;
	std::shared_ptr<RenderTargetDX11> m_RenderTarget;

	bool                               m_bResizePending;  // If the window has to be resized, delay the resizing of the swap chain until the prerender function.

	// DirectX
	ATL::CComPtr<ID3D11Device2>        m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2> m_pDeviceContext;
	ATL::CComPtr<IDXGISwapChain2>      m_pSwapChain;
	ATL::CComPtr<ID3D11Texture2D>      m_pBackBuffer;

	DXGI_SAMPLE_DESC                   m_SampleDesc;      // Used to enable multisampling AA
};