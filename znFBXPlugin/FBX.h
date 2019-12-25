#pragma once

class ZN_API CFBX
{
public:
	CFBX(const std::string& SceneName,  std::shared_ptr<ISceneNode> ParentNode);
	virtual ~CFBX();

	const IBaseManager* GetBaseManager() const;

	std::shared_ptr<ISceneNode> GetSceneNode();

private:
	std::shared_ptr<ISceneNode> m_Node;
	const IBaseManager* m_BaseManager;
};