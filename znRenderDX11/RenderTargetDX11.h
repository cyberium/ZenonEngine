#pragma once

#include "TextureDX11.h"
#include "StructuredBufferDX11.h"

class ZN_API RenderTargetDX11 
	: public IRenderTarget
{
public:
	RenderTargetDX11(IRenderDeviceDX11& RenderDeviceDX11);
	virtual ~RenderTargetDX11();

	virtual void AttachTexture(AttachmentPoint attachment, std::shared_ptr<ITexture> texture) override;
	virtual const std::shared_ptr<ITexture>& GetTexture(AttachmentPoint attachment) override;

	virtual void Clear(AttachmentPoint attachemnt, ClearFlags clearFlags = ClearFlags::All, cvec4 color = vec4(0), float depth = 1.0f, uint8_t stencil = 0) override;
	virtual void Clear(ClearFlags clearFlags = ClearFlags::All, cvec4 color = vec4(0), float depth = 1.0f, uint8_t stencil = 0) override;

	virtual void GenerateMipMaps() override;

	virtual void AttachStructuredBuffer(uint8_t slot, std::shared_ptr<IStructuredBuffer> rwBuffer) override;
	virtual const std::shared_ptr<IStructuredBuffer>& GetStructuredBuffer(uint8_t slot) override;

	virtual void Resize(size_t width, size_t height) override;
	virtual void Bind() override;
	virtual void UnBind() override;
	virtual bool IsValid() const override;

private:
	typedef std::vector<std::shared_ptr<ITexture>> TextureList;
	TextureList m_Textures;

	typedef std::vector<std::shared_ptr<StructuredBufferDX11>> StructuredBufferList;
	StructuredBufferList m_StructuredBuffers;

	uint16_t m_Width;
	uint16_t m_Height;

	// Check to see if the render target is valid.
	bool m_bCheckValidity;

private: // Link to parent d3d11 device
	IRenderDeviceDX11& m_RenderDeviceDX11;
};