#include "stdafx.h"

// General
#include "RenderWindowDX11.h"

// FORWARD BEGIN
static DXGI_RATIONAL QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync);
// FORWARD END

RenderWindowDX11::RenderWindowDX11(IRenderDeviceDX11* RenderDeviceD3D11, IWindowObject * WindowObject, bool vSync)
	: RenderWindowBase(RenderDeviceD3D11->GetRenderDevice(), WindowObject, vSync)
    , m_RenderDeviceD3D11(RenderDeviceD3D11)
	, m_pSwapChain(nullptr)
	, m_pBackBuffer(nullptr)
{
	CreateSwapChain();
}

RenderWindowDX11::~RenderWindowDX11()
{
	if (m_pSwapChain)
	{
		// Apparently an exception is thrown when you release the swap chain if you don't do this.
		m_pSwapChain->SetFullscreenState(false, NULL);
	}
}


void RenderWindowDX11::Present()
{
    GetRenderTarget()->Bind();

	// Copy the render target's color buffer to the swap chain's back buffer.
	TextureDX11* colorBuffer = dynamic_cast<TextureDX11*>(GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::Color0));
	if (colorBuffer)
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->CopyResource(m_pBackBuffer, colorBuffer->GetTextureResource());
	}

	if (IsVSync())
	{
		m_pSwapChain->Present(1, 0);
	}
	else
	{
		m_pSwapChain->Present(0, 0);
	}

}


//
// Protected
//
void RenderWindowDX11::CreateSwapChain()
{
    RenderWindowBase::CreateSwapChain();

    UINT windowWidth = GetWindowWidth();
    UINT windowHeight = GetWindowHeight();
    bool vSync = IsVSync();

    ATL::CComPtr<IDXGIFactory2> factory;
    if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory2), (void**)&factory)))
    {
        Log::Error("Failed to create DXGI factory.");
		return;
    }

	m_SampleDesc = { 1, 0 };

	/*
	UINT sampleCount = 1;
	UINT qualityLevels = 0;
	while (SUCCEEDED(m_DeviceD3D11->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, sampleCount, &qualityLevels)) && qualityLevels > 0)
	{
		// That works...
		m_SampleDesc.Count = sampleCount;
		m_SampleDesc.Quality = qualityLevels - 1;

		// But can we do better?
		sampleCount = sampleCount * 2;
	}
	*/

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = windowWidth;
    swapChainDesc.Height = windowHeight;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Stereo = FALSE;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SampleDesc = m_SampleDesc;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // Use Alt-Enter to switch between full screen and windowed mode.

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapChainFullScreenDesc = {};
    swapChainFullScreenDesc.RefreshRate = QueryRefreshRate(windowWidth, windowHeight, vSync);
    swapChainFullScreenDesc.Windowed = true;

    // First create a DXGISwapChain1
    ATL::CComPtr<IDXGISwapChain1> pSwapChain;
    if (FAILED(factory->CreateSwapChainForHwnd(m_RenderDeviceD3D11->GetDeviceD3D11(), GetWindowObject()->GetHWnd(), &swapChainDesc, &swapChainFullScreenDesc, nullptr, &pSwapChain)))
    {
        Log::Error("Failed to create swap chain.");
    }

    // Now query for the IDXGISwapChain2 interface.
    if (FAILED(pSwapChain->QueryInterface<IDXGISwapChain2>(&m_pSwapChain)))
    {
        Log::Error("Failed to retrieve IDXGISwapChain2 interface.");
    }

    if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer)))
    {
        Log::Error("Failed to get back buffer pointer from swap chain.");
    }

    // Color buffer (Color0)
    ITexture::TextureFormat colorTextureFormat
    (
        ITexture::Components::RGBA,
        ITexture::Type::UnsignedNormalized,
        m_SampleDesc.Count,
        8, 8, 8, 8, 0, 0
    );
    ITexture* colorTexture = GetRenderDevice()->CreateTexture2D(windowWidth, windowHeight, 1, colorTextureFormat);

    // Depth/stencil buffer
    ITexture::TextureFormat depthStencilTextureFormat(
        ITexture::Components::DepthStencil,
        ITexture::Type::UnsignedNormalized,
        m_SampleDesc.Count,
        0, 0, 0, 0, 24, 8);
    ITexture* depthStencilTexture = GetRenderDevice()->CreateTexture2D(windowWidth, windowHeight, 1, depthStencilTextureFormat);

    GetRenderTarget()->AttachTexture(IRenderTarget::AttachmentPoint::Color0, colorTexture);
    GetRenderTarget()->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, depthStencilTexture);
    GetRenderTarget()->Resize(windowWidth, windowHeight);
}

void RenderWindowDX11::ResizeSwapChainBuffers(uint32_t width, uint32_t height)
{
    // If either the width or the height are 0, make them 1.
    width = glm::max<uint32_t>(width, 1);
    height = glm::max<uint32_t>(height, 1);

    //// Make sure we're not referencing the render targets when the window is resized.
    m_RenderDeviceD3D11->GetDeviceContextD3D11()->OMSetRenderTargets(0, nullptr, nullptr);

    // Release the current render target views and texture resources.
    m_pBackBuffer.Release();

    // Resize the swap chain buffers.
    if (FAILED(m_pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0)))
    {
        Log::Error("Failed to resize the swap chain buffer.");
    }

    if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer)))
    {
        Log::Error("Failed to get back buffer pointer from swap chain.");
    }

    // Resize the render target.
    GetRenderTarget()->Resize(width, height);
}




//
// This function was inspired by: http://www.rastertek.com/dx11tut03.html
//
static DXGI_RATIONAL QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync)
{
    DXGI_RATIONAL refreshRate = { 0, 1 };
    if (vsync)
    {
        ATL::CComPtr<IDXGIFactory> factory;
        ATL::CComPtr<IDXGIAdapter> adapter;
        ATL::CComPtr<IDXGIOutput> adapterOutput;
        DXGI_MODE_DESC* displayModeList;

        // Create a DirectX graphics interface factory.
        if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory)))
        {
            Log::Error("Failed to create DXGIFactory");
        }

        if (FAILED(factory->EnumAdapters(0, &adapter)))
        {
            Log::Error("Failed to enumerate adapters.");
        }

        if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))
        {
            Log::Error("Failed to enumerate adapter outputs.");
        }

        UINT numDisplayModes;
        if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, NULL)))
        {
            Log::Error("Failed to query display modes.");
        }

        displayModeList = new DXGI_MODE_DESC[numDisplayModes];
        assert(displayModeList);

        if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModeList)))
        {
            Log::Error("Failed to query dispaly mode list.");
        }

        // Now store the refresh rate of the monitor that matches the width and height of the requested screen.
        for (UINT i = 0; i < numDisplayModes; ++i)
        {
            if (displayModeList[i].Width == screenWidth && displayModeList[i].Height == screenHeight)
            {
                refreshRate = displayModeList[i].RefreshRate;
            }
        }

        delete[] displayModeList;
    }

    return refreshRate;
}