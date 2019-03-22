#pragma once

class RenderDeviceOGL;
class TextureOGL;
class StructuredBufferOGL;

class RenderTargetOGL : public IRenderTarget
{
public:
	RenderTargetOGL(RenderDeviceOGL* _device);
	virtual ~RenderTargetOGL();

	virtual void AttachTexture(AttachmentPoint attachment, std::shared_ptr<Texture> texture);
	virtual std::shared_ptr<Texture> GetTexture(AttachmentPoint attachment);

	virtual void Clear(AttachmentPoint attachemnt, ClearFlags clearFlags = ClearFlags::All, cvec4 color = vec4(0.6f), float depth = 1.0f, uint8_t stencil = 0);
	virtual void Clear(ClearFlags clearFlags = ClearFlags::All, cvec4 color = vec4(0), float depth = 1.0f, uint8_t stencil = 0);
	
	virtual void GenerateMipMaps();

	virtual void AttachStructuredBuffer(uint8_t slot, std::shared_ptr<StructuredBuffer> rwBuffer);
	virtual std::shared_ptr<StructuredBuffer> GetStructuredBuffer(uint8_t slot);

	virtual void Resize(uint16_t width, uint16_t height);

	virtual void Bind();
	virtual void UnBind();
	virtual bool IsValid() const;

private:
	GLuint m_GLObj;

private:
	typedef std::vector< std::shared_ptr<TextureOGL> > TextureList;
	TextureList m_Textures;

	typedef std::vector< std::shared_ptr<StructuredBufferOGL> > StructuredBufferList;
	StructuredBufferList m_StructuredBuffers;

	uint16_t m_Width;
	uint16_t m_Height;
	bool m_bCheckValidity;

	//--
	RenderDeviceOGL* m_RenderDevice;
};