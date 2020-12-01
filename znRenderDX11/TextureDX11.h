#pragma once

class ZN_API TextureDX11 
	: public ITexture
	, public ITextureInternal
	, public Object
	, public std::enable_shared_from_this<TextureDX11>
{
public:
	TextureDX11(IRenderDeviceDX11& RenderDeviceDX11);
	TextureDX11(IRenderDeviceDX11& RenderDeviceDX11, uint16 width, uint16 height, uint16 slices, const TextureFormat& format, EAccess cpuAccess);
	TextureDX11(IRenderDeviceDX11& RenderDeviceDX11, uint16 size, const TextureFormat& format, EAccess cpuAccess);
	virtual ~TextureDX11();

	// ITexture
	const std::string& GetFilename() const override;

	void LoadTexture2D(const std::string& FileName) override;
	void LoadTexture2DFromImage(const std::shared_ptr<IImage>& Image) override;

	void LoadTextureCube(const std::string& FileName) override;
	void LoadTextureCubeFromImages(const std::vector<std::shared_ptr<IImage>>& Images) override;

	void GenerateMipMaps();

	uint16 GetWidth() const override;
	uint16 GetHeight() const override;
	glm::uvec2 GetSize() const override;
	uint16 GetDepth() const override;
	uint8 GetBPP() const override;
	uint8 GetSamplesCount() const override;
	bool IsTransparent() const override;

	virtual void Resize(uint16 width, uint16 height = 0, uint16 depth = 0);
	virtual void Copy(const std::shared_ptr<ITexture>& other);
	virtual void Clear(ClearFlags clearFlags = ClearFlags::All, const glm::vec4& color = glm::vec4(0), float depth = 1.0f, uint8 stencil = 0);

	virtual void Bind(uint32_t ID, const IShader* shader, IShaderParameter::EType parameterType) const override;
	virtual void UnBind(uint32_t ID, const IShader* shader, IShaderParameter::EType parameterType) const override;

	const std::vector<uint8>& GetBuffer() override;

	// ITextureInternal
	void SetFileName(const std::string& FileName) override;

	// Gets the texture resource associated to this texture
	ID3D11Resource* GetTextureResource() const;
	ID3D11ShaderResourceView* GetShaderResourceView() const; // Gets the shader resource view for this texture so that it can be bound to a shader parameter.
	ID3D11DepthStencilView* GetDepthStencilView() const; // Gets the depth stencil view if this is a depth/stencil texture. Otherwise, this function will return null
	ID3D11RenderTargetView* GetRenderTargetView() const; // Get the render target view so the texture can be attached to a render target.
	ID3D11UnorderedAccessView* GetUnorderedAccessView() const; // Get the unordered access view so it can be bound to compute shaders and pixel shaders as a RWTexture

protected:
	virtual void Plot(glm::ivec2 coord, const uint8* pixel, size_t size);
	virtual void FetchPixel(glm::ivec2 coord, uint8*& pixel, size_t size);

	virtual void Resize2D(uint16 width, uint16 height);
	virtual void ResizeCube(uint16 size);

	// Try to choose the best multi-sampling quality level that is supported for the given format.
	DXGI_SAMPLE_DESC GetSupportedSampleCount(DXGI_FORMAT format, uint8 numSamples);

private:
	ATL::CComPtr<ID3D11Texture2D> m_DX11Texture2D;
	ATL::CComPtr<ID3D11Texture3D> m_DX11Texture3D;

	// Use this to map the texture to a shader for reading.
	ATL::CComPtr<ID3D11ShaderResourceView> m_DX11ShaderResourceView;

	// Use this to map the texture to a render target for writing.
	ATL::CComPtr<ID3D11RenderTargetView> m_DX11RenderTargetView;

	// Use this texture as the depth/stencil buffer of a render target.
	ATL::CComPtr<ID3D11DepthStencilView> m_DX11DepthStencilView;

	// Use this texture as a Unordered Acccess View (RWTexture)
	ATL::CComPtr<ID3D11UnorderedAccessView> m_DX11UnorderedAccessView;

	// 1D, 2D, 3D, or Cube
	Dimension m_TextureDimension;
	uint16 m_TextureWidth;
	uint16 m_TextureHeight;
	// For CUBE and 3D textures.
	uint16 m_NumSlices;
	// The requested format for the texture type.
	TextureFormat m_TextureFormat;

	// DXGI texture format support flags
	UINT m_TextureResourceFormatSupport;
	UINT m_DepthStencilViewFormatSupport;
	UINT m_ShaderResourceViewFormatSupport;
	UINT m_RenderTargetViewFormatSupport;
	UINT m_UnorderedAccessViewFormatSupport;

	EAccess m_Access;
	// Set to true if CPU write access is supported.
	bool m_bDynamic;

	DXGI_FORMAT m_TextureResourceFormat;
	DXGI_FORMAT m_DepthStencilViewFormat;
	DXGI_FORMAT m_RenderTargetViewFormat;
	DXGI_FORMAT m_ShaderResourceViewFormat;
	DXGI_FORMAT m_UnorderedAccessViewFormat;

	DXGI_SAMPLE_DESC m_SampleDesc;
	// TRUE if mipmaps are supported on the given texture type.
	bool m_NeedGenerateMipmaps;

	// Bits-per pixel
	uint8 m_BPP; 

	// Number of bytes to next scanline.
	uint16 m_Pitch;

	bool     m_bIsTransparent;

	typedef std::vector<uint8> ColorBuffer;
	ColorBuffer m_Buffer;


	mutable bool m_bIsDirty;

	std::string m_FileName;

private: // Link to parent d3d11 device
	IRenderDeviceDX11& m_RenderDeviceDX11;
};
