#pragma once

// FORWARD BEGIN
ZN_INTERFACE IImage;
ZN_INTERFACE IShader;
ZN_INTERFACE IShaderParameterSource;
// FORWARD END

ZN_INTERFACE ZN_API ITexture
	: public IShaderParameterSource
	, public virtual IObject
{
	enum class ZN_API Dimension
	{
		Texture2D,
		Texture2DArray,
		TextureCube
	};

	// The number of components used to create the texture.
	enum class ZN_API Components
	{
		R,              // One red component.
		RG,             // Red, and green components.
		RGB,            // Red, green, and blue components.
		RGBA,           // Red, green, blue, and alpha components.
		Depth,          // Depth component.
		DepthStencil    // Depth and stencil in the same texture.
	};

	// The type of components in the texture.
	enum class ZN_API Type
	{
		Typeless,           // Typeless formats.
		UnsignedNormalized, // Unsigned normalized (8, 10, or 16-bit unsigned integer values mapped to the range [0..1])
		SignedNormalized,   // Signed normalized (8, or 16-bit signed integer values mapped to the range [-1..1])
		Float,              // Floating point format (16, or 32-bit).
		UnsignedInteger,    // Unsigned integer format (8, 16, or 32-bit unsigned integer formats).
		SignedInteger,      // Signed integer format (8, 16, or 32-bit signed integer formats).
	};

	struct ZN_API TextureFormat
	{
		ITexture::Components Components;
		ITexture::Type Type;

		// For multi-sample textures, we can specify how many samples we want 
		// to use for this texture. Valid values are usually in the range [1 .. 16]
		// depending on hardware support.
		// A value of 1 will effectively disable multisampling in the texture.
		uint8 NumSamples;

		// Components should commonly be 8, 16, or 32-bits but some texture formats
		// support 1, 10, 11, 12, or 24-bits per component.
		uint8 RedBits;
		uint8 GreenBits;
		uint8 BlueBits;
		uint8 AlphaBits;
		uint8 DepthBits;
		uint8 StencilBits;

		// By default create a 4-component unsigned normalized texture with 8-bits per component and no multisampling.
		TextureFormat
		(
			ITexture::Components components = Components::RGBA,
			ITexture::Type type = Type::UnsignedNormalized,
			uint8 numSamples = 1,
			uint8 redBits = 8,
			uint8 greenBits = 8,
			uint8 blueBits = 8,
			uint8 alphaBits = 8,
			uint8 depthBits = 0,
			uint8 stencilBits = 0
		)
			: Components(components)
			, Type(type)
			, NumSamples(numSamples)
			, RedBits(redBits)
			, GreenBits(greenBits)
			, BlueBits(blueBits)
			, AlphaBits(alphaBits)
			, DepthBits(depthBits)
			, StencilBits(stencilBits)
		{}

		// TODO: Define some commonly used texture formats.
	};

	// For cube maps, we may need to access a particular face of the cube map.
	enum class ZN_API CubeFace
	{
		Right,  // +X
		Left,   // -X
		Top,    // +Y
		Bottom, // -Y
		Front,  // +Z
		Back,   // -Z
	};

	virtual ~ITexture() {}

	/**
	 * Get texture Filename (if presented)
	 */
	virtual const std::string& GetFilename() const = 0;

	/**
	 * Load a 2D custom texture
	 */
	virtual bool LoadTextureFromImage(const std::shared_ptr<IImage>& Image) = 0;

	/**
	 * Load a 2D texture from a file path.
	 */
	virtual bool LoadTexture2D(const std::string& fileName) = 0;

	/**
	 * Load a cubemap texture from a file path.
	 */
	virtual bool LoadTextureCube(const std::vector<std::shared_ptr<IImage>>& Images) = 0;

	/**
	 * Generate mip maps for a texture.
	 * For texture formats that don't support mipmapping,
	 * this function does nothing.
	 */
	virtual void GenerateMipMaps() = 0;

	/**
	 * Get a pointer to a particular face of a cubemap texture.
	 * For 1D, and 2D textures, this function always returns the only
	 * face of the texture (the texture itself).
	 */
	//virtual ITexture* GetFace(CubeFace face) const = 0;

	/**
	 * 3D textures store several slices of 2D textures.
	 * Use this function to get a single 2D slice of a 3D texture.
	 * For Cubemaps, this function can be used to get a face of the cubemap.
	 * For 1D and 2D textures, this function will always return the texture
	 * itself.
	 */
	//virtual ITexture* GetSlice(uint32 slice) const = 0;

	virtual uint16_t GetWidth() const = 0;      // Get the width of the textures in texels.
	virtual uint16_t GetHeight() const = 0;     // Get the height of the texture in texles.
	virtual glm::uvec2 GetSize() const = 0;     // Get the 2d size of the texture in texles.
	virtual uint16_t GetDepth() const = 0;      // Get the cube faces for cubemap textures.
	virtual uint8 GetBPP() const = 0;           // Get the bits-per-pixel of the texture.
	virtual uint8 GetSamplesCount() const = 0;  // Get samples count for this texture
	virtual bool IsTransparent() const = 0;     // Check to see if this texture has an alpha channel.

	// Resize the texture to the new dimensions.
	// Resizing a texture will cause the original texture to be discarded.
	// Only use with "dynamic" textures (not ones loaded from a texture file).
	// @param width The width of the texture (for 1D, 2D, and 3D textures or size of a cubemap face for Cubemap textures)
	// @param height The height of the texture (for 2D, 3D textures)
	// @param depth The depth of the texture (for 3D textures only)
	virtual void Resize(uint16_t width, uint16_t height = 0, uint16_t depth = 0) = 0;

	/**
	 * Plot a color to the texture.
	 * This method is only valid for texture created with EAccess::Write access.
	 * @param coord The non-normalized texture coordinate.
	 * @param color The color to plot (RGBA).
	 */
	virtual void Plot(glm::ivec2 coord, const uint8* pixel, size_t size) = 0;

	/**
	 * Retrieve the pixel at a particular location in the
	 * texture.
	 * This method is only valid for textures created with EAccess::Read access.
	 * @param coord The non-normalized texture coordinate.
	 * @return The pixel cast to the requested type.
	 */
	virtual void FetchPixel(glm::ivec2 coord, uint8*& pixel, size_t size) = 0;

	/**
	 * Copy the contents of one texture into this one.
	 * Textures must both be the same size and have compatible types.
	 * @see https://msdn.microsoft.com/en-us/library/windows/desktop/ff476392(v=vs.85).aspx
	 * @see https://www.opengl.org/sdk/docs/man/html/glCopyTexSubImage2D.xhtml
	 */
	virtual void Copy(const std::shared_ptr<ITexture>& other) = 0;

	/**
	 * Clear the texture.
	 * @param color The color to clear the texture to.
	 * @param depth The depth value to use for depth textures.
	 * @param stencil The stencil value to use for depth/stencil textures.
	 */
	virtual void Clear(ClearFlags clearFlags = ClearFlags::All, const glm::vec4& color = glm::vec4(0), float depth = 1.0f, uint8 stencil = 0) = 0;

	/**
	 * Bind this texture for use by the shaders.
	 */
	virtual void Bind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const = 0;
	virtual void Bind(uint32_t ID, EShaderType _shaderType, IShaderParameter::Type parameterType) const = 0;

	/**
	 * Unbind the texture.
	 */
	virtual void UnBind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const = 0;
	virtual void UnBind(uint32_t ID, EShaderType _shaderType, IShaderParameter::Type parameterType) const = 0;

	/**
	 * Get texture data
	 */
	virtual const std::vector<uint8>& GetBuffer() = 0;

	// Template specizalizations

	template< typename T >
	inline void Plot(glm::ivec2 coord, const T& color)
	{
		Plot(coord, reinterpret_cast<const uint8*>(&color), sizeof(T));
	}

	template< typename T >
	inline T FetchPixel(glm::ivec2 coord)
	{
		uint8* pixel = nullptr;
		FetchPixel(coord, pixel, sizeof(T));

		return *reinterpret_cast<T*>(pixel);
	}
};

ZN_INTERFACE ZN_API ITextureInternal
{
	virtual ~ITextureInternal() {}

	virtual void SetFileName(const std::string& FileName) = 0;
};

typedef std::vector<std::shared_ptr<ITexture>> TextureList;
typedef std::unordered_map<uint8, std::shared_ptr<ITexture>> TextureMap;
typedef std::unordered_map<std::string, std::shared_ptr<ITexture>> TextureNameMap;
