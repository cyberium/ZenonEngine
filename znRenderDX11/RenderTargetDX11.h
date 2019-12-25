#pragma once

class TextureDX11;
class StructuredBufferDX11;

class ZN_API RenderTargetDX11 : public IRenderTarget
{
public:
	RenderTargetDX11(ID3D11Device2* pDevice);
	virtual ~RenderTargetDX11();

	virtual void AttachTexture(AttachmentPoint attachment, std::shared_ptr<ITexture> texture);
	virtual std::shared_ptr<ITexture> GetTexture(AttachmentPoint attachment);

	virtual void Clear(AttachmentPoint attachemnt, ClearFlags clearFlags = ClearFlags::All, cvec4 color = vec4(0), float depth = 1.0f, uint8_t stencil = 0);
	virtual void Clear(ClearFlags clearFlags = ClearFlags::All, cvec4 color = vec4(0), float depth = 1.0f, uint8_t stencil = 0);

	virtual void GenerateMipMaps();

	virtual void AttachStructuredBuffer(uint8_t slot, std::shared_ptr<IStructuredBuffer> rwBuffer);
	virtual std::shared_ptr<IStructuredBuffer> GetStructuredBuffer(uint8_t slot);

	virtual void Resize(uint16_t width, uint16_t height);
	virtual void Bind();
	virtual void UnBind();
	virtual bool IsValid() const;


private:
	typedef std::vector< std::shared_ptr<TextureDX11> > TextureList;
	TextureList m_Textures;

	typedef std::vector< std::shared_ptr<StructuredBufferDX11> > StructuredBufferList;
	StructuredBufferList m_StructuredBuffers;

	uint16_t m_Width;
	uint16_t m_Height;

	// Check to see if the render target is valid.
	bool m_bCheckValidity;

	// DirectX
	ATL::CComPtr<ID3D11Device2> m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2> m_pDeviceContext;
};