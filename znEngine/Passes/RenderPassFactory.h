#pragma once

class CRenderPassFactory
	: public IRenderPassFactory
	, public IznPluginsEventListener
{
public:
	CRenderPassFactory();
	virtual ~CRenderPassFactory();

	// IRenderPassFactory
	void AddRenderPassCreator(std::shared_ptr<IRenderPassCreator> Creator) override;
	void RemoveRenderPassCreator(std::shared_ptr<IRenderPassCreator> Creator) override;
	std::shared_ptr<IRenderPass> CreateRenderPass(std::string RenderPassName, IRenderDevice& RenderDevice, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport, std::shared_ptr<IScene> Scene) const override;

	// IznPluginsEventListener
	void OnPluginAdded(std::shared_ptr<IznPlugin> Plugin) override;
	void OnPluginInitialized(std::shared_ptr<IznPlugin> Plugin) override;
	void OnPluginFinalized(std::shared_ptr<IznPlugin> Plugin) override;

private:
	std::vector<std::shared_ptr<IRenderPassCreator>> m_Creators;
};