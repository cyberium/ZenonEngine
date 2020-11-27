#pragma once

// FORWARD BEGIN
ZN_INTERFACE IRenderPass;
ZN_INTERFACE IRenderPassCreateTypelessList;
// FORWARD END

ZN_INTERFACE ZN_API IRenderer
{
	virtual ~IRenderer() {}

    virtual void Add3DPass(std::shared_ptr<IRenderPass> pass) = 0;
	virtual void AddUIPass(std::shared_ptr<IRenderPass> pass) = 0;

	virtual void Render3D(RenderEventArgs& renderEventArgs) = 0;
	virtual void RenderUI(RenderEventArgs& renderEventArgs) = 0;

	virtual void Resize(glm::ivec2 Size) = 0;
};


ZN_INTERFACE ZN_API IRendererExtender
{
	virtual ~IRendererExtender() {}

	virtual void Extend3DPasses(IRenderer& Renderer, IRenderDevice& RenderDevice, IScene& Scene,                                                           std::shared_ptr<IRenderTarget> RenderTarget) const = 0;
	virtual void Extend3DPasses(IRenderer& Renderer, IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& SceneNodeListPass, std::shared_ptr<IRenderTarget> RenderTarget) const = 0;
};