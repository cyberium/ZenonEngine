#pragma once

class RenderDeviceOGL;

/**
 * Texture implementation for DX11.
 */

class TextureOGL : public Texture, public std::enable_shared_from_this<TextureOGL>
{
public:
	// Create an empty texture.
	TextureOGL(RenderDeviceOGL* _device);

	// 2D Texture
	TextureOGL(RenderDeviceOGL* _device, uint16_t width, uint16_t height, uint16_t slices, const TextureFormat& format, CPUAccess cpuAccess);

	// Cube Texture
	TextureOGL(RenderDeviceOGL* _device, uint16_t size, uint16_t count,                    const TextureFormat& format, CPUAccess cpuAccess);

	virtual ~TextureOGL();

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

	virtual uint16_t GetWidth() const;
	virtual uint16_t GetHeight() const;
	virtual uint16_t GetDepth() const;
	virtual uint8_t GetBPP() const; 	// Get the bits-per-pixel of the texture.
	virtual bool IsTransparent() const; // Check to see if this texture has an alpha channel.

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
	virtual const std::vector<uint8>& GetBuffer();

	/**
	 * Gets the OpenGL object associated to this texture
	 */
	uint32 GetGLObject();

protected:
	virtual void Plot(glm::ivec2 coord, const uint8_t* pixel, size_t size);
	virtual void FetchPixel(glm::ivec2 coord, uint8_t*& pixel, size_t size);

	virtual void Resize2D(uint16_t width, uint16_t height);
	virtual void ResizeCube(uint16_t size);

private:
	GLenum m_TextureType;
	GLuint m_GLObj;

private:
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

	bool m_bIsDirty;
};