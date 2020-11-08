#pragma once

// FORWARD BEGIN
class RenderEventArgs;
ZN_INTERFACE IRenderTarget;
ZN_INTERFACE IPipelineState;
class Viewport;
// FORWARD END

ZN_INTERFACE ZN_API IRenderPass 
{
	virtual ~IRenderPass() {}

    virtual void SetEnabled(bool enabled) = 0;
    virtual bool IsEnabled() const = 0;

    virtual void PreRender(RenderEventArgs& e) = 0;
    virtual void Render(RenderEventArgs& e) = 0;
    virtual void PostRender(RenderEventArgs& e) = 0;
};


ZN_INTERFACE ZN_API IRenderPassPipelined 
	: public virtual IRenderPass
{
	virtual ~IRenderPassPipelined() {}

	virtual std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) = 0;
	virtual const IPipelineState& GetPipeline() const = 0;
};



ZN_INTERFACE ZN_API IRenderPassMultipipelinded
	: public virtual IRenderPass
{
	virtual ~IRenderPassMultipipelinded() {}

	virtual void Configure(std::shared_ptr<IRenderTarget> OutputRenderTarget) = 0;
	virtual const std::vector<std::shared_ptr<IPipelineState>>& GetPipelines() const = 0;
};




/*

//
// For plugins
//
ZN_INTERFACE ZN_API IRenderPassCreator
{
	virtual ~IRenderPassCreator() {}

	virtual size_t                       GetRenderPassCount() const = 0;
	virtual std::string                  GetRenderPassName(size_t Index) const = 0;
	virtual std::shared_ptr<IRenderPass> CreateRenderPass(size_t Index, IRenderDevice& RenderDevice, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport, std::shared_ptr<IScene> Scene) const = 0;
};



ZN_INTERFACE ZN_API 
	__declspec(uuid("A0ED56A7-77B5-40E4-B479-1CA95CAAD96C")) IRenderPassFactory
	: public IManager
{
	virtual ~IRenderPassFactory() {}

	virtual void AddRenderPassCreator(std::shared_ptr<IRenderPassCreator> Creator) = 0;
	virtual void RemoveRenderPassCreator(std::shared_ptr<IRenderPassCreator> Creator) = 0;

	virtual std::shared_ptr<IRenderPass> CreateRenderPass(std::string RenderPassName, IRenderDevice& RenderDevice, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport, std::shared_ptr<IScene> Scene) const = 0;
};

*/
