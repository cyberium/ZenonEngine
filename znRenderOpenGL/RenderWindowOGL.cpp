#include <stdafx.h>

// Include
#include "RenderDeviceOGL.h"

// General
#include "RenderWindowOGL.h"

// Additional
#include "RenderTargetOGL.h"
#include "TextureOGL.h"

RenderWindowOGL::RenderWindowOGL(std::shared_ptr<RenderDeviceOGL> device, IWindowObject * WindowObject, bool vSync)
	: RenderWindow(WindowObject, vSync)
	, m_RenderDevice(device)
	, m_bResizePending(false)
{
	m_HDC = GetDC(GetHWnd());

	m_RenderDevice.lock()->CreateDevice(m_HDC);
	m_RenderDevice.lock()->LoadDefaultResources();

    m_RenderTarget = std::dynamic_pointer_cast<RenderTargetOGL>(m_RenderDevice.lock()->CreateRenderTarget());

    CreateSwapChain();
}

RenderWindowOGL::~RenderWindowOGL()
{
}

void RenderWindowOGL::ResizeSwapChainBuffers(uint32_t width, uint32_t height)
{
	width = glm::max<uint32_t>(width, 1);
	height = glm::max<uint32_t>(height, 1);

	m_RenderTarget->Resize(width, height);
}

void RenderWindowOGL::Present()
{
	std::shared_ptr<TextureOGL> colorBuffer = std::dynamic_pointer_cast<TextureOGL>(m_RenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0));
	if (colorBuffer)
	{
		//m_pDeviceContext->CopyResource(m_pBackBuffer, colorBuffer->GetTextureResource());
	}

	if (IsVSync())
	{
        if (!SwapBuffers(m_HDC))
            fail1();
	}
	else
	{
        if (!SwapBuffers(m_HDC))
            fail1();
	}
}

std::shared_ptr<IRenderTarget> RenderWindowOGL::GetRenderTarget()
{
	return m_RenderTarget;
}

void RenderWindowOGL::CreateSwapChain()
{
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
    m_RenderTarget->Resize(windowWidth2, windowHeight2);
}



//
// Engine events
//
void RenderWindowOGL::OnPreRender(RenderEventArgs& e)
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

void RenderWindowOGL::OnPostRender(RenderEventArgs& e)
{
    //m_RenderTarget->UnBind();

	base::OnPostRender(e);
}



//
// Window events
//
void RenderWindowOGL::OnResize(ResizeEventArgs& e)
{
	base::OnResize(e);

    m_bResizePending = true;
}
