#pragma once

#ifdef ZN_FBX_SDK_ENABLE
#include <fbxsdk.h>
#include "FBXInterfaces.h"

class CFBXScene 
	: public IFBXScene
	, public IFBXScenePrivate
	, public std::enable_shared_from_this<CFBXScene>
{
public:
	CFBXScene(const IBaseManager& BaseManager, fbxsdk::FbxManager* FBXManager);
	virtual ~CFBXScene();

	bool LoadFromFile(std::shared_ptr<IFile> File);
	std::string GetPath() const;

	// IFBXScene
	std::shared_ptr<IFBXNode> GetRootNode() const override;
	const std::vector<std::shared_ptr<IFBXModel>>& GetModels() const override;
	std::shared_ptr<IFBXSkeleton> GetSkeleton() const override;
	std::shared_ptr<IFBXAnimation> GetAnimation() const override;

	// IFBXScenePrivate
	void AddModel(const std::shared_ptr<IFBXModel>& Model) override;

private:
	std::string m_Path;
	fbxsdk::FbxScene* m_NativeScene;
	std::shared_ptr<IFBXNode> m_RootNode;
	std::vector<std::shared_ptr<IFBXModel>> m_Models;
	std::shared_ptr<IFBXSkeleton> m_Skeleton;
	std::shared_ptr<IFBXAnimation> m_Animation;
private:
	const IBaseManager& m_BaseManager;
};

#endif
