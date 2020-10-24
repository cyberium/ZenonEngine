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
	CFBXScene(const IBaseManager& BaseManager, fbxsdk::FbxManager* FBXManager, const std::string& TexturePath);
	virtual ~CFBXScene();

	bool LoadFromFile(std::shared_ptr<IFile> File);
	std::string GetPath() const;

	// IFBXScene
	std::shared_ptr<IFBXNode> GetFBXRootNode() const override;
	const std::vector<std::shared_ptr<IFBXModel>>& GetFBXModels() const override;
	std::shared_ptr<IFBXSkeleton> GetFBXSkeleton() const override;
	std::shared_ptr<IFBXAnimation> GetFBXAnimation() const override;

	virtual const std::string& GetTexturesPath() const override;

	std::shared_ptr<IModel> MergeModels() override;

	// IFBXScenePrivate
	void AddModel(const std::shared_ptr<IFBXModel>& Model) override;

private:
	std::string m_Path;
	std::shared_ptr<IFBXNode> m_RootNode;
	std::vector<std::shared_ptr<IFBXModel>> m_Models;
	std::shared_ptr<IFBXSkeleton> m_Skeleton;
	std::shared_ptr<IFBXAnimation> m_Animation;

	std::shared_ptr<IModel> m_MergedModel;
private:
	const IBaseManager& m_BaseManager;
	fbxsdk::FbxScene* m_NativeScene;
	std::string m_TexturesPath;
};

#endif
