#pragma once

#ifdef ZN_FBX_SDK_ENABLE
#include <fbxsdk.h>
#include "FBXInterfaces.h"

class CFBXScene 
	: public IFBXScene
	, public std::enable_shared_from_this<CFBXScene>
{
public:
	CFBXScene(const IBaseManager& BaseManager, fbxsdk::FbxManager* FBXManager);
	virtual ~CFBXScene();

	bool LoadFromFile(std::shared_ptr<IFile> File);

public:
	std::string GetPath() const;
	std::shared_ptr<IFBXNode> GetRootNode() const override;
	std::shared_ptr<IFBXSkeleton> GetSkeleton() const override;
	std::shared_ptr<IFBXAnimation> GetAnimation() const override;

private:
	std::string m_Path;
	fbxsdk::FbxScene* m_NativeScene;
	std::shared_ptr<IFBXNode> m_RootNode;
	std::shared_ptr<IFBXSkeleton> m_Skeleton;
	std::shared_ptr<IFBXAnimation> m_Animation;
private:
	const IBaseManager& m_BaseManager;
};

#endif
