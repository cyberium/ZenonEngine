#pragma once

#ifdef ZN_FBX_SDK_ENABLE
#include <fbxsdk.h>
#include "FBXInterfaces.h"

class CFBXManager
	: public IFBXManager
{
public:
	CFBXManager(const IBaseManager& BaseManager);
	virtual ~CFBXManager();

	// IFBXManager
	std::shared_ptr<IFBXScene> LoadFBX(const std::string& FileName) override;

private:
	fbxsdk::FbxManager* m_FBXManager;
	const IBaseManager& m_BaseManager;
};

#endif
