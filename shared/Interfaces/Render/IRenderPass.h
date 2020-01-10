#pragma once

// FORWARD BEGIN
class RenderEventArgs;
class Viewport;
// FORWARD END

ZN_INTERFACE ZN_API IRenderPass : public IVisitor
{
	virtual ~IRenderPass() {}

    // Enable or disable the pass. If a pass is disabled, the technique will skip it.
    virtual void SetEnabled(bool enabled) = 0;
    virtual bool IsEnabled() const = 0;

    // Render the pass. This should only be called by the RenderTechnique.
    virtual void PreRender(RenderEventArgs& e) = 0;
    virtual void Render(RenderEventArgs& e) = 0;
    virtual void PostRender(RenderEventArgs& e) = 0;

    // Update viewport (need for texture resizing)
    virtual void UpdateViewport(const Viewport * _viewport) = 0;
};


ZN_INTERFACE ZN_API IRenderScenePass : public IRenderPass
{
	virtual ~IRenderScenePass() {}

	
};



//
// For plugins
//
ZN_INTERFACE ZN_API IRenderPassCreator
{
	virtual ~IRenderPassCreator() {}

	virtual size_t                       GetRenderPassCount() const = 0;
	virtual std::string                  GetRenderPassName(size_t Index) const = 0;
	virtual std::shared_ptr<IRenderPass> CreateRenderPass(size_t Index, std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport, std::shared_ptr<IScene> Scene) const = 0;
};



ZN_INTERFACE ZN_API
__declspec(uuid("A0ED56A7-77B5-40E4-B479-1CA95CAAD96C"))
IRenderPassFactory
	: public IManager
{
	virtual ~IRenderPassFactory() {}

	virtual void AddRenderPassCreator(std::shared_ptr<IRenderPassCreator> Creator) = 0;
	virtual void RemoveRenderPassCreator(std::shared_ptr<IRenderPassCreator> Creator) = 0;

	virtual std::shared_ptr<IRenderPass> CreateRenderPass(std::string RenderPassName, std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport, std::shared_ptr<IScene> Scene) const = 0;
};
