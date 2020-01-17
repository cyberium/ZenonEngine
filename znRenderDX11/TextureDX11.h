#pragma once

class ZN_API TextureDX11 : public ITexture, public std::enable_shared_from_this<TextureDX11>
{
public:
	TextureDX11(std::weak_ptr<IRenderDevice> RenderDevice);
	TextureDX11(std::weak_ptr<IRenderDevice> RenderDevice, uint16_t width, uint16_t height, uint16_t slices, const TextureFormat& format, CPUAccess cpuAccess, bool bUAV = false);
	TextureDX11(std::weak_ptr<IRenderDevice> RenderDevice, uint16_t size, uint16_t count, const TextureFormat& format, CPUAccess cpuAccess, bool bUAV = false);

	virtual ~TextureDX11();

	virtual bool LoadTextureCustom(uint16_t width, uint16_t height, void* pixels);
	virtual bool LoadTexture2D(const std::string& fileName);
	virtual bool LoadTextureCube(const std::string& fileName);

	virtual void GenerateMipMaps();

	virtual std::shared_ptr<ITexture> GetFace(CubeFace face) const;
	virtual std::shared_ptr<ITexture> GetSlice(uint32 slice) const;
	virtual uint16_t GetWidth() const;
	virtual uint16_t GetHeight() const;
	glm::ivec2 GetSize() const;
	virtual uint16_t GetDepth() const;
	virtual uint8_t GetBPP() const;
	virtual bool IsTransparent() const;

	virtual void Resize(uint16_t width, uint16_t height = 0, uint16_t depth = 0);
	virtual void Copy(std::shared_ptr<ITexture> other);
	virtual void Clear(ClearFlags clearFlags = ClearFlags::All, cvec4 color = vec4(0), float depth = 1.0f, uint8_t stencil = 0);

	virtual void Bind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const override;
	virtual void Bind(uint32_t ID, EShaderType _shaderType, IShaderParameter::Type parameterType) const override;

	virtual void UnBind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const override;
	virtual void UnBind(uint32_t ID, EShaderType _shaderType, IShaderParameter::Type parameterType) const override;

	const std::vector<uint8>& GetBuffer();

	// Gets the texture resource associated to this texture
	ID3D11Resource* GetTextureResource() const;

	// Gets the shader resource view for this texture so that it can be bound to a shader parameter.
	ID3D11ShaderResourceView* GetShaderResourceView() const;

	// Gets the depth stencil view if this is a depth/stencil texture. Otherwise, this function will return null
	ID3D11DepthStencilView* GetDepthStencilView() const;

	// Get the render target view so the texture can be attached to a render target.
	ID3D11RenderTargetView* GetRenderTargetView() const;

	// Get the unordered access view so it can be bound to compute shaders and pixel shaders as a RWTexture
	ID3D11UnorderedAccessView* GetUnorderedAccessView() const;

protected:
	virtual void Plot(glm::ivec2 coord, const uint8_t* pixel, size_t size);
	virtual void FetchPixel(glm::ivec2 coord, uint8_t*& pixel, size_t size);

	virtual void Resize2D(uint16_t width, uint16_t height);
	virtual void ResizeCube(uint16_t size);

	// Try to choose the best multi-sampling quality level that is supported for the given format.
	DXGI_SAMPLE_DESC GetSupportedSampleCount(DXGI_FORMAT format, uint8_t numSamples);

private:
	std::weak_ptr<IRenderDevice> m_RenderDevice;

	ATL::CComPtr<ID3D11Device2> m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2> m_pDeviceContext;

	ATL::CComPtr<ID3D11Texture2D> m_pTexture2D;
	ATL::CComPtr<ID3D11Texture3D> m_pTexture3D;

	// Use this to map the texture to a shader for reading.
	ATL::CComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;

	// Use this to map the texture to a render target for writing.
	ATL::CComPtr<ID3D11RenderTargetView> m_pRenderTargetView;

	// Use this texture as the depth/stencil buffer of a render target.
	ATL::CComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

	// Use this texture as a Unordered Acccess View (RWTexture)
	ATL::CComPtr<ID3D11UnorderedAccessView> m_pUnorderedAccessView;

	// 1D, 2D, 3D, or Cube
	Dimension m_TextureDimension;
	uint16_t m_TextureWidth;
	uint16_t m_TextureHeight;
	// For CUBE and 3D textures.
	uint16_t m_NumSlices;
	// The requested format for the texture type.
	TextureFormat m_TextureFormat;

	// DXGI texture format support flags
	UINT m_TextureResourceFormatSupport;
	UINT m_DepthStencilViewFormatSupport;
	UINT m_ShaderResourceViewFormatSupport;
	UINT m_RenderTargetViewFormatSupport;
	UINT m_UnorderedAccessViewFormatSupport;

	CPUAccess m_CPUAccess;
	// Set to true if CPU write access is supported.
	bool m_bDynamic;
	// This resource should be used as a UAV.
	bool m_bUAV;

	DXGI_FORMAT m_TextureResourceFormat;
	DXGI_FORMAT m_DepthStencilViewFormat;
	DXGI_FORMAT m_RenderTargetViewFormat;
	DXGI_FORMAT m_ShaderResourceViewFormat;
	DXGI_FORMAT m_UnorderedAccessViewFormat;

	DXGI_SAMPLE_DESC m_SampleDesc;
	// TRUE if mipmaps are supported on the given texture type.
	bool m_bGenerateMipmaps;
	// Bits-per pixel
	uint8_t m_BPP; 

	// Number of bytes to next scanline.
	uint16_t m_Pitch;

	bool     m_bIsTransparent;

	typedef std::vector<uint8_t> ColorBuffer;
	ColorBuffer m_Buffer;

	std::string m_TextureFileName;

	mutable bool m_bIsDirty;
};