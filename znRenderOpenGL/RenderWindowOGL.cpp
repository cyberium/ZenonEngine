#include <stdafx.h>

// Include
#include "RenderDeviceOGL.h"

// General
#include "RenderWindowOGL.h"

// Additional
#include "RenderTargetOGL.h"
#include "TextureOGL.h"

RenderWindowOGL::RenderWindowOGL(HWND hWnd, std::shared_ptr<RenderDeviceOGL> device, cstring windowName, int windowWidth, int windowHeight, bool vSync)
	: RenderWindow(windowName, windowWidth, windowHeight, hWnd, vSync)
	, m_bIsMouseTracking(false)
	, m_RenderDevice(device)
	, m_bResizePending(false)
{
	m_HDC = GetDC(hWnd);

	m_RenderDevice.lock()->CreateDevice(m_HDC);
	m_RenderDevice.lock()->LoadDefaultResources();

	// Create a render target for the back buffer and depth/stencil buffers.
	m_RenderTarget = std::dynamic_pointer_cast<RenderTargetOGL>(m_RenderDevice.lock()->CreateRenderTarget());

	// Create the device and swap chain before the window is shown.
	UINT windowWidth2 = GetWindowWidth();
	UINT windowHeight2 = GetWindowHeight();
	bool vSync2 = IsVSync();

	std::string windowName2 = GetWindowName();

	// Depth/stencil buffer
	Texture::TextureFormat depthStencilTextureFormat(
		Texture::Components::Depth,
		Texture::Type::UnsignedNormalized,
		1, //m_SampleDesc.Count,
		0, 0, 0, 0, 24, 0);
	std::shared_ptr<Texture> depthStencilTexture = m_RenderDevice.lock()->CreateTexture2D(windowWidth2, windowHeight2, 1, depthStencilTextureFormat);

	// Color buffer (Color0)
	Texture::TextureFormat colorTextureFormat
	(
		Texture::Components::RGBA,
		Texture::Type::UnsignedNormalized,
		1, //m_SampleDesc.Count,
		8, 8, 8, 8, 0, 0
	);
	std::shared_ptr<Texture> colorTexture = m_RenderDevice.lock()->CreateTexture2D(windowWidth2, windowHeight2, 1, colorTextureFormat);

	m_RenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color0, colorTexture);
	m_RenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Depth, depthStencilTexture);
}

RenderWindowOGL::~RenderWindowOGL()
{
	/*if (m_pSwapChain)
	{
		// Apparently an exception is thrown when you release the swap chain if you don't do this.
		m_pSwapChain->SetFullscreenState(false, NULL);
	}*/
}

void RenderWindowOGL::ResizeSwapChainBuffers(uint32_t width, uint32_t height)
{
	// If either the width or the height are 0, make them 1.
	width = glm::max<uint32_t>(width, 1);
	height = glm::max<uint32_t>(height, 1);

	//// Make sure we're not referencing the render targets when the window is resized.
	/*m_pDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

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
	}*/

	// Resize the render target.
	m_RenderTarget->Resize(width, height);
}

void RenderWindowOGL::Present()
{
	// The application can leave the render targets unbound.
	// Bind the render window's default render target before 
	// drawing AntTweakBar.
	//m_RenderTarget->Bind();

	// Copy the render target's color buffer to the swap chain's back buffer.
	std::shared_ptr<TextureOGL> colorBuffer = std::dynamic_pointer_cast<TextureOGL>(m_RenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0));
	if (colorBuffer)
	{
		//m_pDeviceContext->CopyResource(m_pBackBuffer, colorBuffer->GetTextureResource());
	}

	if (IsVSync())
	{
		//m_pSwapChain->Present(1, 0);
		SwapBuffers(m_HDC);
	}
	else
	{
		//m_pSwapChain->Present(0, 0);
		SwapBuffers(m_HDC);
	}
}

std::shared_ptr<IRenderTarget> RenderWindowOGL::GetRenderTarget()
{
	return m_RenderTarget;
}

void RenderWindowOGL::OnPreRender(Render3DEventArgs& e)
{
	// Get the currently bound frame buffer object to avoid reset to invalid FBO
	GLint defaultRB = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultRB);
	OGLCheckError();
	m_RenderDevice.lock()->SetDefaultRB(defaultRB);

	if (m_bResizePending)
	{
		ResizeSwapChainBuffers(GetWindowWidth(), GetWindowHeight());
		m_bResizePending = false;
	}

	m_RenderTarget->Bind();

	base::OnPreRender(e);
}

void RenderWindowOGL::OnPostRender(Render3DEventArgs& e)
{
	base::OnPostRender(e);
}

void RenderWindowOGL::OnMouseMoved(MouseMotionEventArgs& e)
{
	if (!m_bIsMouseTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = GetHWND();
		if (TrackMouseEvent(&tme))
		{
			m_bIsMouseTracking = true;
		}
	}

	base::OnMouseMoved(e);
}

void RenderWindowOGL::OnMouseLeave(EventArgs& e)
{
	m_bIsMouseTracking = false;

	base::OnMouseLeave(e);
}

void RenderWindowOGL::OnResize(ResizeEventArgs& e)
{
	// This function can be called a lot very quickly.
	// Delay the resizing of the swap chain until the OnPreRender function is called
	// again (which doesn't happen until the user finishes resizing the window.
//    ResizeSwapChainBuffers( e.Width, e.Height );
	base::OnResize(e);

	// The swap chain will be resized the next time OnPreRender is invoked.
	m_bResizePending = true;
}

void RenderWindowOGL::OnTerminate(EventArgs& e)
{
	base::OnTerminate(e);
}
