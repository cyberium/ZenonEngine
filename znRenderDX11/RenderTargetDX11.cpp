#include "stdafx.h"

// General
#include "RenderTargetDX11.h"

namespace
{
	D3D11_RECT TranslateRect(const Rect& rect)
	{
		D3D11_RECT d3dRect = { 0 };
		d3dRect.top = static_cast<LONG>(glm::round(rect.Y));
		d3dRect.bottom = static_cast<LONG>(glm::round(rect.Y + rect.Height));
		d3dRect.left = static_cast<LONG>(glm::round(rect.X));
		d3dRect.right = static_cast<LONG>(glm::round(rect.X + rect.Width));
		return d3dRect;
	}

	D3D11_VIEWPORT TranslateViewport(const Viewport& viewport)
	{
		D3D11_VIEWPORT d3dViewport = { 0 };
		d3dViewport.TopLeftX = viewport.GetX();
		d3dViewport.TopLeftY = viewport.GetY();
		d3dViewport.Width = viewport.GetWidth();
		d3dViewport.Height = viewport.GetHeight();
		d3dViewport.MinDepth = viewport.GetMinDepth();
		d3dViewport.MaxDepth = viewport.GetMaxDepth();
		return d3dViewport;
	}
}

RenderTargetDX11::RenderTargetDX11(IRenderDeviceDX11& RenderDeviceDX11)
	: m_RenderDeviceDX11(RenderDeviceDX11)
	, m_Width(0)
	, m_Height(0)
	, m_bCheckValidity(false)
	, m_ViewportDirty(true)
	, m_ScissorDirty(true)
{
	m_Textures.resize((size_t)IRenderTarget::AttachmentPoint::NumAttachmentPoints + 1);
	m_StructuredBuffers.resize(8);
}

RenderTargetDX11::~RenderTargetDX11()
{

}

void RenderTargetDX11::AttachTexture(AttachmentPoint attachment, std::shared_ptr<ITexture> texture)
{
	m_Textures[(uint8_t)attachment] = texture;
	m_bCheckValidity = true;
}

const std::shared_ptr<ITexture>& RenderTargetDX11::GetTexture(AttachmentPoint attachment)
{
	return m_Textures.at((uint8_t)attachment);
}

void RenderTargetDX11::AttachStructuredBuffer(uint8_t slot, std::shared_ptr<IStructuredBuffer> rwBuffer)
{
	std::shared_ptr<StructuredBufferDX11> rwbufferDX11 = std::dynamic_pointer_cast<StructuredBufferDX11>(rwBuffer);
	m_StructuredBuffers[slot] = rwbufferDX11;

	m_bCheckValidity = true;
}

const std::shared_ptr<IStructuredBuffer>& RenderTargetDX11::GetStructuredBuffer(uint8_t slot)
{
	return m_StructuredBuffers[slot];
}

void RenderTargetDX11::GenerateMipMaps()
{
	for (const auto& texture : m_Textures)
	{
		if (texture)
		{
			texture->GenerateMipMaps();
		}
	}
}

void RenderTargetDX11::Clear(AttachmentPoint attachment, ClearFlags clearFlags, cvec4 color, float depth, uint8_t stencil)
{
	const std::shared_ptr<ITexture>& texture = m_Textures[(uint8_t)attachment];
	if (texture)
	{
		texture->Clear(clearFlags, color, depth, stencil);
	}
}

void RenderTargetDX11::Clear(ClearFlags clearFlags, cvec4 color, float depth, uint8_t stencil)
{
	for (uint8_t i = 0; i < (uint8_t)AttachmentPoint::NumAttachmentPoints; ++i)
	{
		Clear((AttachmentPoint)i, clearFlags, color, depth, stencil);
	}
}

void RenderTargetDX11::SetViewport(const Viewport& viewport)
{
	m_Viewport = viewport;
	m_ViewportDirty = true;
}
const Viewport& RenderTargetDX11::GetViewport() const
{
	return m_Viewport;
}

void RenderTargetDX11::SetScissorRect(const Rect& rect)
{
	m_Scissor = rect;
	m_ScissorDirty = true;
}

const Rect& RenderTargetDX11::GetScissorRect() const
{
	return m_Scissor;
}

void RenderTargetDX11::Resize(size_t width, size_t height)
{
	if (m_Width != width || m_Height != height)
	{
		Log::Green("RenderTargetResized: %d, %d", width, height);

		m_Width = glm::max<size_t>(width, 1);
		m_Height = glm::max<size_t>(height, 1);

		// Resize the attached textures.
		for (const auto& texture : m_Textures)
		{
			if (texture)
			{
				texture->Resize(m_Width, m_Height);
			}
		}

		m_Viewport.SetWidth(m_Width);
		m_Viewport.SetHeight(m_Height);
		m_ViewportDirty = true;
	}
}

void RenderTargetDX11::Bind()
{
	if (m_bCheckValidity)
	{
		if (!IsValid())
		{
			throw CException("Invalid render target.");
		}
		m_bCheckValidity = false;
	}

	ID3D11RenderTargetView* renderTargetViews[8];
	UINT numRTVs = 0;
	for (uint8_t i = 0; i < 8; i++)
		if (const auto& texture = m_Textures[i])
			renderTargetViews[numRTVs++] = std::dynamic_pointer_cast<TextureDX11>(texture)->GetRenderTargetView();

	ID3D11UnorderedAccessView* uavViews[8];
	UINT uavStartSlot = numRTVs;
	UINT numUAVs = 0;
	for (uint8_t i = 0; i < 8; i++)
		if (const auto& rwbuffer = m_StructuredBuffers[i])
			uavViews[numUAVs++] = std::dynamic_pointer_cast<StructuredBufferDX11>(rwbuffer)->GetUnorderedAccessView();

	ID3D11DepthStencilView* depthStencilView = nullptr;
	if (const auto& depthTexture = m_Textures[(uint8_t)AttachmentPoint::Depth])
	{
		depthStencilView = std::dynamic_pointer_cast<TextureDX11>(depthTexture)->GetDepthStencilView();
	}
	else if (const auto& depthStencilTexture = m_Textures[(uint8_t)AttachmentPoint::DepthStencil])
	{
		depthStencilView = std::dynamic_pointer_cast<TextureDX11>(depthStencilTexture)->GetDepthStencilView();
	}

	m_RenderDeviceDX11.GetDeviceContextD3D11()->OMSetRenderTargetsAndUnorderedAccessViews(numRTVs, renderTargetViews, depthStencilView, uavStartSlot, numUAVs, uavViews, nullptr);

	// Viewport & Scissor
	{
		if (m_ViewportDirty)
		{
			m_ViewportRectD3D = TranslateViewport(m_Viewport);
			m_ViewportDirty = false;
		}

		if (m_ScissorDirty)
		{
			m_ScissorRectD3D = TranslateRect(m_Scissor);
			m_ScissorDirty = false;
		}

		m_RenderDeviceDX11.GetDeviceContextD3D11()->RSSetViewports(1, &m_ViewportRectD3D);
		m_RenderDeviceDX11.GetDeviceContextD3D11()->RSSetScissorRects(1, &m_ScissorRectD3D);
	}
}

void RenderTargetDX11::UnBind()
{
	m_RenderDeviceDX11.GetDeviceContextD3D11()->RSSetViewports(0, NULL);
	m_RenderDeviceDX11.GetDeviceContextD3D11()->RSSetScissorRects(0, NULL);

	m_RenderDeviceDX11.GetDeviceContextD3D11()->OMSetRenderTargetsAndUnorderedAccessViews(0, NULL, NULL, 0, 0, NULL, NULL);
}


//
// Protected
//
bool RenderTargetDX11::IsValid() const
{
	UINT numRTV = 0;
	int width = -1;
	int height = -1;

	for (const auto& texture : m_Textures)
	{
		if (texture)
		{
			if (std::dynamic_pointer_cast<TextureDX11>(texture)->GetRenderTargetView())
				++numRTV;

			if (width == -1 || height == -1)
			{
				width = texture->GetWidth();
				height = texture->GetHeight();
			}
			else
			{
				if (texture->GetWidth() != width || texture->GetHeight() != height)
				{
					return false;
				}
			}
		}
	}

	UINT numUAV = 0;
	for (auto rwBuffer : m_StructuredBuffers)
	{
		if (rwBuffer)
		{
			++numUAV;
		}
	}

	if (numRTV + numUAV > 8)
	{
		return false;
	}

	return true;
}
