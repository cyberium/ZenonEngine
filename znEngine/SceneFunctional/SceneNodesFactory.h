#pragma once

class CSceneNodesFactory
	: public ISceneNodesFactory
	, public IznPluginsEventListener
{
public:
	CSceneNodesFactory(IBaseManager* BaseManager);
	virtual ~CSceneNodesFactory();

	// ISceneNodesFactory
	void AddSceneNodeCreator(std::shared_ptr<ISceneNodeCreator> Creator) override;
	void RemoveSceneNodeCreator(std::shared_ptr<ISceneNodeCreator> Creator) override;

	ISceneNode3D* CreateSceneNode3D(ISceneNode3D* Parent, std::string SceneNodeTypeName) const override;
	ISceneNodeUI* CreateSceneNodeUI(ISceneNodeUI* Parent, std::string SceneNodeTypeName) const override;

	// IznPluginsEventListener
	void OnPluginAdded(std::shared_ptr<IznPlugin> Plugin) override;
	void OnPluginInitialized(std::shared_ptr<IznPlugin> Plugin) override;

private:
	std::vector<std::shared_ptr<ISceneNodeCreator>> m_Creators;
};