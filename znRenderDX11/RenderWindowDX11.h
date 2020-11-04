#pragma once

#include "TextureDX11.h"
#include "RenderTargetDX11.h"

class ZN_API RenderWindowDX11 
	: public RenderWindowBase
{
public:
	RenderWindowDX11(IRenderDeviceDX11& RenderDeviceDX11, std::unique_ptr<IznNativeWindow> WindowObject, bool vSync);
	virtual ~RenderWindowDX11();

	// IRenderWindow
	bool IsVSync() const override;
	void Present() override;

protected:
	IRenderDevice& GetRenderDevice() const override final;
    void CreateSwapChain() override final;
	void ResizeSwapChainBuffers(uint32_t width, uint32_t height) override final;

private:
	ATL::CComPtr<IDXGISwapChain4> m_pSwapChain;
	ATL::CComPtr<ID3D11Texture2D> m_pBackBuffer;

	DXGI_SAMPLE_DESC m_SampleDesc;      // Used to enable multisampling AA

	bool m_vSync;

private: // Link to parent d3d11 device
	IRenderDeviceDX11& m_RenderDeviceDX11;
};