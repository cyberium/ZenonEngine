#pragma once

/**
 * Texture implementation for DX11.
 */

class TextureDX11 : public Texture, public std::enable_shared_from_this<TextureDX11>
{
public:
	// Create an empty texture.
	TextureDX11(ID3D11Device2* pDevice);

	// 2D Texture
	TextureDX11(ID3D11Device2* pDevice, uint16_t width, uint16_t height, uint16_t slices, const TextureFormat& format, CPUAccess cpuAccess, bool bUAV = false);

	// Cube Texture
	TextureDX11(ID3D11Device2* pDevice, uint16_t size, uint16_t count, const TextureFormat& format, CPUAccess cpuAccess, bool bUAV = false);

	virtual ~TextureDX11();

	/**
	 * Load a 2D custom texture
	 */
	virtual bool LoadTextureCustom(uint16_t width, uint16_t height, void* pixels);

	/**
	 * Load a 2D texture from a file path.
	 */
	virtual bool LoadTexture2D(cstring fileName);

	/**
	 * Load a cubemap texture from a file path.
	 */
	virtual bool LoadTextureCube(cstring fileName);

	/**
	* Generate mip maps for a texture.
	* For texture formats that don't support mipmapping,
	* this function does nothing.
	*/
	virtual void GenerateMipMaps();

	/**
	* Get a pointer to a particular face of a cubemap texture.
	* For 1D, and 2D textures, this function always returns the only
	* face of the texture (the texture itself).
	*/
	virtual std::shared_ptr<Texture> GetFace(CubeFace face) const;

	/**
	* 3D textures store several slices of 2D textures.
	* Use this function to get a single 2D slice of a 3D texture.
	* For Cubemaps, this function can be used to get a face of the cubemap.
	* For 1D and 2D textures, this function will always return the texture
	* itself.
	*/
	virtual std::shared_ptr<Texture> GetSlice(uint32 slice) const;

	// Get the width of the textures in texels.
	virtual uint16_t GetWidth() const;

	// Get the height of the texture in texles.
	virtual uint16_t GetHeight() const;

	// Get the 2D size of the texture in texles
	glm::ivec2 GetSize() const;

	// Get the depth of the texture in texels.
	virtual uint16_t GetDepth() const;

	// Get the bits-per-pixel of the texture.
	virtual uint8_t GetBPP() const;

	// Check to see if this texture has an alpha channel.
	virtual bool IsTransparent() const;

	// Resize the texture to the new dimensions.
	// Resizing a texture will cause the original texture to be discarded.
	// Only use on "dynamic" textures (not ones loaded from a texture file).
	// @param width The width of the texture (for 1D, 2D, and 3D textures or size of a cubemap face for Cubemap textures)
	// @param height The height of the texture (for 2D, 3D textures)
	// @param depth The depth of the texture (for 3D textures only)
	virtual void Resize(uint16_t width, uint16_t height = 0, uint16_t depth = 0);

	/**
	 * Copy the contents of one texture into another.
	 * Textures must both be the same size.
	 */
	virtual void Copy(std::shared_ptr<Texture> other);

	/**
	* Clear the texture.
	* @param color The color to clear the texture to.
	* @param depth The depth value to use for depth textures.
	* @param stencil The stencil value to use for depth/stencil textures.
	*/
	virtual void Clear(ClearFlags clearFlags = ClearFlags::All, cvec4 color = vec4(0), float depth = 1.0f, uint8_t stencil = 0);

	/**
	 * Bind this texture for use by the shaders.
	 */
	virtual void Bind(uint32_t ID, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType);
	virtual void Bind(uint32_t ID, Shader::ShaderType _shaderType, ShaderParameter::Type parameterType);

	/**
	 * Unbind the texture.
	 */
	virtual void UnBind(uint32_t ID, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType);
	virtual void UnBind(uint32_t ID, Shader::ShaderType _shaderType, ShaderParameter::Type parameterType);

	/**
	 * Get texture data
	 */
	const std::vector<uint8>& GetBuffer();

	// Gets the texture resource associated to this texture
	ID3D11Resource* GetTextureResource() const;

	// Gets the shader resource view for this texture so that it can be 
	// bound to a shader parameter.
	ID3D11ShaderResourceView* GetShaderResourceView() const;

	// Gets the depth stencil view if this is a depth/stencil texture.
	// Otherwise, this function will return null
	ID3D11DepthStencilView* GetDepthStencilView() const;

	// Get the render target view so the texture can be attached to a render target.
	ID3D11RenderTargetView* GetRenderTargetView() const;

	// Get the unordered access view so it can be bound to compute shaders and 
	// pixel shaders as a RWTexture
	ID3D11UnorderedAccessView* GetUnorderedAccessView() const;

protected:
	virtual void Plot(glm::ivec2 coord, const uint8_t* pixel, size_t size);
	virtual void FetchPixel(glm::ivec2 coord, uint8_t*& pixel, size_t size);

	virtual void Resize2D(uint16_t width, uint16_t height);
	virtual void ResizeCube(uint16_t size);

	// Try to choose the best multi-sampling quality level that is supported for the given format.
	DXGI_SAMPLE_DESC GetSupportedSampleCount(DXGI_FORMAT format, uint8_t numSamples);

private:
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

	bool m_bIsDirty;
};