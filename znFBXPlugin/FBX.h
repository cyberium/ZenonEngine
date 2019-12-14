#pragma once

class OW_ENGINE_API CFBX
{
public:
	CFBX(const std::string& SceneName,  std::shared_ptr<ISceneNode> ParentNode, std::shared_ptr<IMaterial> DefaultMaterial);
	virtual ~CFBX();

	const IBaseManager* GetBaseManager() const;

	std::shared_ptr<ISceneNode> GetSceneNode();
	std::shared_ptr<IMaterial> GetDefaultMaterial();

private:
	std::shared_ptr<ISceneNode> m_Node;
	const IBaseManager* m_BaseManager;
	std::shared_ptr<IMaterial> m_DefaultMaterial;
};