#pragma once

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
	virtual bool LoadTextureCustom(uint16_t width, uint16_t height, void* pixels) override final;

	/**
	 * Load a 2D texture from a file path.
	 */
	virtual bool LoadTexture2D(const std::string& fileName) override final;

	/**
	 * Load a cubemap texture from a file path.
	 */
	virtual bool LoadTextureCube(const std::string& fileName) override final;

	/**
	* Generate mip maps for a texture.
	* For texture formats that don't support mipmapping,
	* this function does nothing.
	*/
	virtual void GenerateMipMaps() override final;

	/**
	* Get a pointer to a particular face of a cubemap texture.
	* For 1D, and 2D textures, this function always returns the only
	* face of the texture (the texture itself).
	*/
	virtual std::shared_ptr<ITexture> GetFace(CubeFace face) const override final;

	/**
	* 3D textures store several slices of 2D textures.
	* Use this function to get a single 2D slice of a 3D texture.
	* For Cubemaps, this function can be used to get a face of the cubemap.
	* For 1D and 2D textures, this function will always return the texture
	* itself.
	*/
	virtual std::shared_ptr<ITexture> GetSlice(uint32 slice) const override final;

	virtual uint16_t GetWidth() const override final;
	virtual uint16_t GetHeight() const override final;
    glm::ivec2 GetSize() const override final;
	virtual uint16_t GetDepth() const override final;
	virtual uint8_t GetBPP() const override final; 	// Get the bits-per-pixel of the texture.
	virtual bool IsTransparent() const override final; // Check to see if this texture has an alpha channel.

	// Resize the texture to the new dimensions.
	// Resizing a texture will cause the original texture to be discarded.
	// Only use on "dynamic" textures (not ones loaded from a texture file).
	// @param width The width of the texture (for 1D, 2D, and 3D textures or size of a cubemap face for Cubemap textures)
	// @param height The height of the texture (for 2D, 3D textures)
	// @param depth The depth of the texture (for 3D textures only)
	virtual void Resize(uint16_t width, uint16_t height = 0, uint16_t depth = 0) override final;

	/**
	 * Copy the contents of one texture into another.
	 * Textures must both be the same size.
	 */
	virtual void Copy(std::shared_ptr<ITexture> other) override final;

	/**
	* Clear the texture.
	* @param color The color to clear the texture to.
	* @param depth The depth value to use for depth textures.
	* @param stencil The stencil value to use for depth/stencil textures.
	*/
	virtual void Clear(ClearFlags clearFlags = ClearFlags::All, cvec4 color = vec4(0), float depth = 1.0f, uint8_t stencil = 0) override final;

	/**
	 * Bind this texture for use by the shaders.
	 */
	virtual void Bind(uint32_t ID, const Shader* shader, ShaderParameter::Type parameterType) const override final;
	virtual void Bind(uint32_t ID, Shader::ShaderType _shaderType, ShaderParameter::Type parameterType) const override final;

	/**
	 * Unbind the texture.
	 */
	virtual void UnBind(uint32_t ID, const Shader* shader, ShaderParameter::Type parameterType) const override final;
	virtual void UnBind(uint32_t ID, Shader::ShaderType _shaderType, ShaderParameter::Type parameterType) const override final;

	/**
	 * Get texture data
	 */
	virtual const std::vector<uint8>& GetBuffer() override final;

	/**
	 * Gets the OpenGL object associated to this texture
	 */
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