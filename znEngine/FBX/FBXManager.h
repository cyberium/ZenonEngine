#pragma once

#ifdef ZN_FBX_SDK_ENABLE

#include <fbxsdk.h>

#include "FBXInterfaces.h"
#include "FBXScene.h"
#include "FBXSceneNode.h"

class CFBXManager
	: public IFBXManager
{
public:
	CFBXManager(const IBaseManager& BaseManager);
	virtual ~CFBXManager();

	// IFBXManager
	fbxsdk::FbxManager* GetFBXManager() const override;

private:
	fbxsdk::FbxManager* m_FBXManager;
	const IBaseManager& m_BaseManager;
};

#endif
