#pragma once

#if 0

class CScenesFactory
	: public IScenesFactory
	, public IznPluginsEventListener
{
public:
	CScenesFactory(IBaseManager* BaseManager);
	virtual ~CScenesFactory();

	// IScenesFactory
	void AddSceneCreator(std::shared_ptr<ISceneCreator> Creator) override;
	void RemoveSceneCreator(std::shared_ptr<ISceneCreator> Creator) override;
	std::shared_ptr<IScene> CreateScene(std::string SceneTypeName) const override;

	// IznPluginsEventListener
	void OnPluginAdded(std::shared_ptr<IznPlugin> Plugin) override;
	void OnPluginInitialized(std::shared_ptr<IznPlugin> Plugin) override;

private:
	std::vector<std::shared_ptr<ISceneCreator>> m_Creators;
};

#endif
