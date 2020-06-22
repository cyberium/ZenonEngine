#include "stdafx.h"

// General
#include "RenderWindowDX11.h"

// FORWARD BEGIN
static DXGI_RATIONAL QueryRefreshRate(UINT screenWidth, UINT screenHeight, BOOL vsync);
// FORWARD END

RenderWindowDX11::RenderWindowDX11(IRenderDeviceDX11& RenderDeviceDX11, INativeWindow& WindowObject, bool vSync)
	: RenderWindowBase(WindowObject, vSync)
    , m_RenderDeviceDX11(RenderDeviceDX11)
	, m_pSwapChain(nullptr)
	, m_pBackBuffer(nullptr)
{
	WindowObject.SetEventsListener(this);

	CreateSwapChain();
}

RenderWindowDX11::~RenderWindowDX11()
{
	if (m_pSwapChain)
	{
		// Apparently an exception is thrown when you release the swap chain if you don't do this.
		CHECK_HR(m_pSwapChain->SetFullscreenState(false, NULL));
	}

	m_NativeWindow.ResetEventsListener();
}


void RenderWindowDX11::Present()
{
	m_RenderTarget->Bind();

	// Copy the render target's color buffer to the swap chain's back buffer.
	std::shared_ptr<ITexture> colorTexture = m_RenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0);
	std::shared_ptr<TextureDX11> colorTextureDX11 = std::dynamic_pointer_cast<TextureDX11>(colorTexture);
	m_RenderDeviceDX11.GetDeviceContextD3D11()->CopyResource(m_pBackBuffer, colorTextureDX11->GetTextureResource());

	CHECK_HR(m_pSwapChain->Present(IsVSync() ? 1 : 0, 0));
}



//
// Protected
//
IRenderDevice& RenderWindowDX11::GetRenderDevice() const
{
	return m_RenderDeviceDX11;
}

void RenderWindowDX11::CreateSwapChain()
{
    RenderWindowBase::CreateSwapChain();

    UINT windowWidth = GetWindowWidth();
    UINT windowHeight = GetWindowHeight();
    bool vSync = IsVSync();

    ATL::CComPtr<IDXGIFactory4> factory;
	CHECK_HR_MSG(CreateDXGIFactory(__uuidof(IDXGIFactory4), (void**)&factory), L"Failed to create DXGI factory.");

	m_SampleDesc = { 1, 0 };

	UINT sampleCount = 1;
	UINT qualityLevels = 0;
	while (SUCCEEDED(m_RenderDeviceDX11.GetDeviceD3D11()->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, sampleCount, &qualityLevels)) && qualityLevels > 0)
	{
		m_SampleDesc.Count = sampleCount;
		m_SampleDesc.Quality = qualityLevels - 1;

		sampleCount = sampleCount * 2; 		// But can we do better?
	}
	
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

	INativeWindow_WindowsSpecific& nativeWindow_WindowsSpecific = dynamic_cast<INativeWindow_WindowsSpecific&>(m_NativeWindow);

    // First create a DXGISwapChain1
    ATL::CComPtr<IDXGISwapChain1> pSwapChain;
	CHECK_HR_MSG(factory->CreateSwapChainForHwnd(m_RenderDeviceDX11.GetDeviceD3D11(), nativeWindow_WindowsSpecific.GetHWnd(), &swapChainDesc, &swapChainFullScreenDesc, nullptr, &pSwapChain), L"Failed to create swap chain.");

    // Now query for the IDXGISwapChain2 interface.
	CHECK_HR_MSG(pSwapChain->QueryInterface<IDXGISwapChain4>(&m_pSwapChain), L"Failed to retrieve IDXGISwapChain interface.");

	CHECK_HR_MSG(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer), L"Failed to get back buffer pointer from swap chain.");

    // Color buffer (Color0)
    ITexture::TextureFormat colorTextureFormat
    (
        ITexture::Components::RGBA,
        ITexture::Type::UnsignedNormalized,
        m_SampleDesc.Count,
        8, 8, 8, 8, 0, 0
    );
    std::shared_ptr<ITexture> colorTexture = m_RenderDeviceDX11.GetObjectsFactory().CreateTexture2D(windowWidth, windowHeight, 1, colorTextureFormat);

    // Depth/stencil buffer
    ITexture::TextureFormat depthStencilTextureFormat(
        ITexture::Components::DepthStencil,
        ITexture::Type::UnsignedNormalized,
        m_SampleDesc.Count,
        0, 0, 0, 0, 24, 8);
	std::shared_ptr<ITexture> depthStencilTexture = m_RenderDeviceDX11.GetObjectsFactory().CreateTexture2D(windowWidth, windowHeight, 1, depthStencilTextureFormat);

    m_RenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color0, colorTexture);
	m_RenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, depthStencilTexture);
	m_RenderTarget->SetViewport(GetViewport());
	//m_RenderTarget->Resize(windowWidth, windowHeight);
}

void RenderWindowDX11::ResizeSwapChainBuffers(uint32_t width, uint32_t height)
{
    // If either the width or the height are 0, make them 1.
    width = glm::max<uint32_t>(width, 1);
    height = glm::max<uint32_t>(height, 1);

    // Make sure we're not referencing the render targets when the window is resized.
    m_RenderDeviceDX11.GetDeviceContextD3D11()->OMSetRenderTargets(0, nullptr, nullptr);

    m_pBackBuffer.Release();

	CHECK_HR_MSG(m_pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0), L"Failed to resize the swap chain buffer.");

	CHECK_HR_MSG(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer), L"Failed to get back buffer pointer from swap chain.");
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
		CHECK_HR_MSG(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory), L"Failed to create DXGIFactory");

		ATL::CComPtr<IDXGIAdapter> adapter;
		CHECK_HR_MSG(factory->EnumAdapters(0, &adapter), L"Failed to enumerate adapters.");

		ATL::CComPtr<IDXGIOutput> adapterOutput;
		CHECK_HR_MSG(adapter->EnumOutputs(0, &adapterOutput), L"Failed to enumerate adapter outputs.");

        UINT numDisplayModes = 0;
		CHECK_HR_MSG(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, NULL), L"Failed to query display modes.");

		DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numDisplayModes];
        _ASSERT(displayModeList != nullptr);

		CHECK_HR_MSG(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModeList), L"Failed to query dispaly mode list.");

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