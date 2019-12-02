#pragma once

#include "TextureDX11.h"
#include "RenderTargetDX11.h"

class OW_ENGINE_API RenderWindowDX11 : public RenderWindowBase
{
public:
	RenderWindowDX11(std::shared_ptr<IRenderDevice> RenderDevice, IWindowObject * WindowObject, bool vSync);
	virtual ~RenderWindowDX11();

	void                                            Present() override final;

protected:
    void                                            CreateSwapChain() override final;
	void                                            ResizeSwapChainBuffers(uint32_t width, uint32_t height) override final;

private:
	ATL::CComPtr<ID3D11Device2>                     m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2>              m_pDeviceContext;
	ATL::CComPtr<IDXGISwapChain2>                   m_pSwapChain;
	ATL::CComPtr<ID3D11Texture2D>                   m_pBackBuffer;

	DXGI_SAMPLE_DESC                                m_SampleDesc;      // Used to enable multisampling AA
};