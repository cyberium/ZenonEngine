#pragma once

class CznRenderDeviceFactory 
	: public IznRenderDeviceFactory
	, public IznPluginsEventListener
{
public:
	CznRenderDeviceFactory(IBaseManager* BaseManager);
	virtual ~CznRenderDeviceFactory();

	// IznRenderDeviceFactory
	void RegisterRenderDeviceCreator(std::shared_ptr<IznRenderDeviceCreator> RenderDeviceCreator);
	void UnregisterRenderDeviceCreator(std::shared_ptr<IznRenderDeviceCreator> RenderDeviceCreator);
	const IznRenderDeviceCreator& GetRenderDeviceCreator(RenderDeviceType RenderDeviceType) const;

	// IznPluginsEventListener
	void OnPluginAdded(std::shared_ptr<IznPlugin> Plugin);
	void OnPluginInitialized(std::shared_ptr<IznPlugin> Plugin);
	void OnPluginFinalized(std::shared_ptr<IznPlugin> Plugin);

private:
	std::vector<const IznRenderDeviceCreator*> m_Creators;

private:
	IBaseManager* m_BaseManager;
};