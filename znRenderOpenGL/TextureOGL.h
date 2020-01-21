#pragma once

class TextureOGL : public ITexture, public std::enable_shared_from_this<TextureOGL>
{
public:
	TextureOGL(IRenderDevice* RenderDevice);
	TextureOGL(IRenderDevice* RenderDevice, uint16_t width, uint16_t height, uint16_t slices, const TextureFormat& format, CPUAccess cpuAccess);
	TextureOGL(IRenderDevice* RenderDevice, uint16_t size, uint16_t count,                    const TextureFormat& format, CPUAccess cpuAccess);
	virtual ~TextureOGL();

	virtual bool LoadTextureCustom(uint16_t width, uint16_t height, void* pixels) override final;
	virtual bool LoadTexture2D(const std::string& fileName) override final;
	virtual bool LoadTextureCube(const std::string& fileName) override final;

	virtual void GenerateMipMaps() override final;

	virtual ITexture* GetFace(CubeFace face) const override final;
	virtual ITexture* GetSlice(uint32 slice) const override final;

	virtual uint16_t GetWidth() const override final;
	virtual uint16_t GetHeight() const override final;
    glm::ivec2 GetSize() const override final;
	virtual uint16_t GetDepth() const override final;
	virtual uint8_t GetBPP() const override final;
	virtual bool IsTransparent() const override final;

	virtual void Resize(uint16_t width, uint16_t height = 0, uint16_t depth = 0) override final;
	virtual void Copy(ITexture* other) override final;
	virtual void Clear(ClearFlags clearFlags = ClearFlags::All, cvec4 color = vec4(0), float depth = 1.0f, uint8_t stencil = 0) override final;

	virtual void Bind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const override final;
	virtual void Bind(uint32_t ID, EShaderType _shaderType, IShaderParameter::Type parameterType) const override final;

	virtual void UnBind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const override final;
	virtual void UnBind(uint32_t ID, EShaderType _shaderType, IShaderParameter::Type parameterType) const override final;

	virtual const std::vector<uint8>& GetBuffer() override final;

	uint32 GetGLObject();

protected:
	virtual void Plot(glm::ivec2 coord, const uint8_t* pixel, size_t size) override final;
	virtual void FetchPixel(glm::ivec2 coord, uint8_t*& pixel, size_t size) override final;

	virtual void Resize2D(uint16_t width, uint16_t height);
	virtual void ResizeCube(uint16_t size);

private:
	GLenum m_TextureType;
	GLuint m_GLObj;

private:
	IRenderDevice* m_RenderDevice;

	// 2D or Cube
	Dimension m_TextureDimension;
	uint16_t m_TextureWidth;
	uint16_t m_TextureHeight;
	// For CUBE textures.
	uint16_t m_TextureDepth;
	// Bits-per pixel
	uint8_t m_BPP;
	bool    m_bIsTransparent;
	// The requested format for the texture type.
	TextureFormat m_TextureFormat;

	CPUAccess m_CPUAccess;
	bool m_bDynamic; // Set to true if CPU write access is supported.
	
	// TRUE if mipmaps are supported on the given texture type.
	bool m_bGenerateMipmaps;

	// Number of bytes to next scanline.
	uint16_t m_Pitch;

	typedef std::vector<uint8_t> ColorBuffer;
	ColorBuffer m_Buffer;

	std::string m_TextureFileName;

	mutable bool m_bIsDirty;
};