#pragma once

#include "FBXScene.h"

class ZN_API CFBX
{
public:
	CFBX(const std::string& SceneName,  std::shared_ptr<ISceneNode> ParentNode);
	virtual ~CFBX();

	const IBaseManager* GetBaseManager() const;
	std::shared_ptr<ISceneNode> GetSceneNode();

private:
	std::shared_ptr<CFBXScene> m_FBXScene;

private:
	const IBaseManager* m_BaseManager;
};