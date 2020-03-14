#pragma once

// FORWARD BEGIN
ZN_INTERFACE IStructuredBuffer;
// FORWARD END

ZN_INTERFACE ZN_API IRenderTarget
{
	enum class ZN_API AttachmentPoint
	{
		Color0,         // Must be a uncompressed color format.
		Color1,         // Must be a uncompressed color format.
		Color2,         // Must be a uncompressed color format.
		Color3,         // Must be a uncompressed color format.
		Color4,         // Must be a uncompressed color format.
		Color5,         // Must be a uncompressed color format.
		Color6,         // Must be a uncompressed color format.
		Color7,         // Must be a uncompressed color format.
		Depth,          // Must be a texture with a depth format.
		DepthStencil,   // Must be a texture with a depth/stencil format.
		NumAttachmentPoints
	};

	virtual ~IRenderTarget() {}

	/**
	 * Attach a texture to the render target.
	 * The dimension of all textures attached to a render target
	 * must match.
	 *
	 * To remove a texture from an attachment point, just attach a NULL texture.
	 *
	 * The render target will be validated anytime RenderTarget::Bind is invoked.
	 * You can also check to see if a render target is valid by calling RenderTarget::IsValid
	 * A render target that is valid for one platform may not be valid for another
	 * (for example, if the maximum number of texture slots are exceeded).
	 * Check the documentation for the API you are using for details.
	 * @see https://msdn.microsoft.com/en-us/library/windows/desktop/ff476465(v=vs.85).aspx
	 * @see https://www.opengl.org/sdk/docs/man/html/glFramebufferTexture.xhtml
	 */
	virtual void AttachTexture(AttachmentPoint attachment, std::shared_ptr<ITexture> Texture) = 0;
	virtual const std::shared_ptr<ITexture>& GetTexture(AttachmentPoint attachment) = 0;

	/**
	 * Clear the contents of a texture attached to a specific attachment point.
	 * @param attachemnt The attachment point of which to clear the contents of the texture.
	 * @param clearFlags Which values should be cleared.
	 * @param color The clear color to use for color attachment points.
	 * @param depth The depth value to use for depth attachment points.
	 * @param stencil The stencil value to use for stencil attachment points.
	 */
	virtual void Clear(AttachmentPoint attachemnt, ClearFlags clearFlags = ClearFlags::All, cvec4 color = vec4(0), float depth = 1.0f, uint8_t stencil = 0) = 0;

	/**
	* Clear the contents of all of the textures attached to the render target.
	* @param clearFlags Which values should be cleared.
	* @param color The clear color to use for color attachment points.
	* @param depth The depth value to use for depth attachment points.
	* @param stencil The stencil value to use for stencil attachment points.
	*/
	virtual void Clear(ClearFlags clearFlags = ClearFlags::All, cvec4 color = vec4(0), float depth = 1.0f, uint8_t stencil = 0) = 0;

	/**
	 * Generate mipmaps for all of the textures that are attached to the render target.
	 */
	virtual void GenerateMipMaps() = 0;

	/**
	 * StructuredBuffers can be written to in a shader. StructuredBuffers must be bound to the
	 * rendering pipeline at the same time as render target textures and depth stencil buffers.
	 * The maximum number of StructuredBuffers that can be attached to a render target
	 * are 8 - num color textures. So there can only be a total of 8 color textures
	 * and RWbuffers attached to the render target at any time.
	 */
	virtual void AttachStructuredBuffer(uint8_t slot, std::shared_ptr<IStructuredBuffer> rwBuffer) = 0;
	virtual const std::shared_ptr < IStructuredBuffer>& GetStructuredBuffer(uint8_t slot) = 0;

	/**
	* Specify an array of viewports to bind to the rasterizer stage.
	* The helper function SetViewport can be used to set the viewport at index 0.
	*/
	virtual void SetViewport(const Viewport& viewport) = 0;
	virtual const Viewport& GetViewport() const = 0;

	/**
	 * If scissor rectangle culling is enabled, then use the rectangles passed
	 * in the rects array to perform scissor culling. One scissor rectangle
	 * is specified per viewport.
	 * If the user sends more than GL_MAX_VIEWPORTS (for OpenGL) or ?? for DirectX
	 * scissor rects then only the first GL_MAX_VIEWPORTS or ?? will be set.
	 * Rectangle coordinates are in window coordinates.
	 */
	virtual void SetScissorRect(const Rect& rect) = 0;
	virtual const Rect& GetScissorRect() const = 0;

	/**
	 * Resize the color and depth/stencil textures that are associated to this render target view.
	 * Resizing a texture will clear it's contents.
	 */
	virtual void Resize(size_t width, size_t height) = 0;

	/**
	 * Bind this render target to the rendering pipeline.
	 * It will remain the active render target until another RenderTarget is bound
	 * using this same method.
	 */
	virtual void Bind() = 0;

	/**
	 * Unbind this render target from the rendering pipeline.
	 */
	virtual void UnBind() = 0;
};

typedef std::vector<std::shared_ptr<IRenderTarget>> RenderTargetList;
