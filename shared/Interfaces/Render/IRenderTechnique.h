#pragma once

// FORWARD BEGIN
ZN_INTERFACE IRenderPass;
// FORWARD END

ZN_INTERFACE ZN_API IRenderer
{
	virtual ~IRenderer() {}

    virtual uint32 AddPass(std::shared_ptr<IRenderPass> pass) = 0;
	virtual IRenderPass* GetPass(uint32 ID) const = 0;
	virtual std::shared_ptr<IRenderTarget> GetRenderTarget() const = 0;

	virtual void Render3D(RenderEventArgs& renderEventArgs) = 0;
	virtual void RenderUI(RenderEventArgs& renderEventArgs) = 0;

	virtual void Resize(uint32 NewWidth, uint32 NewHeight) = 0;
};