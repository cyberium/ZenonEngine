#pragma once

#include "TextureDX11.h"
#include "StructuredBufferDX11.h"

class ZN_API RenderTargetDX11 
	: public IRenderTarget
{
public:
	RenderTargetDX11(IRenderDeviceDX11& RenderDeviceDX11);
	virtual ~RenderTargetDX11();

	void AttachTexture(AttachmentPoint attachment, std::shared_ptr<ITexture> texture) override;
	const std::shared_ptr<ITexture>& GetTexture(AttachmentPoint attachment) override;

	void AttachStructuredBuffer(uint8_t slot, std::shared_ptr<IStructuredBuffer> rwBuffer) override;
	const std::shared_ptr<IStructuredBuffer>& GetStructuredBuffer(uint8_t slot) override;

	void GenerateMipMaps() override;

	void Clear(AttachmentPoint attachemnt, ClearFlags clearFlags = ClearFlags::All, cvec4 color = vec4(0), float depth = 1.0f, uint8_t stencil = 0) override;
	void Clear(ClearFlags clearFlags = ClearFlags::All, cvec4 color = vec4(0), float depth = 1.0f, uint8_t stencil = 0) override;

	void SetViewport(const Viewport& viewport) override;
	const Viewport& GetViewport() const override;

	void SetScissorRect(const Rect& rect) override;
	const Rect& GetScissorRect() const override;

	void Resize(size_t width, size_t height) override;
	void Bind() override;
	void UnBind() override;

protected:
	/**
	 * After attaching color, depth, stencil, and StructuredBuffers to the render target,
	 * you can check if the render target is valid using this method.
	 * The render target will also be checked for validity before it is bound
	 * to rendering pipeline (using the RenderTarget::Bind method).
	 */
	bool IsValid() const;

private:
	TextureList m_Textures;

	typedef std::vector<std::shared_ptr<IStructuredBuffer>> StructuredBufferList;
	StructuredBufferList m_StructuredBuffers;

	Viewport                                        m_Viewport;
	Rect                                            m_Scissor;

	bool                                            m_ViewportDirty;
	bool                                            m_ScissorDirty;

	D3D11_VIEWPORT                                  m_ViewportRectD3D;
	D3D11_RECT                                      m_ScissorRectD3D;

	uint16_t                                        m_Width;
	uint16_t                                        m_Height;

	bool                                            m_bCheckValidity;

private: // Link to parent d3d11 device
	IRenderDeviceDX11& m_RenderDeviceDX11;
};