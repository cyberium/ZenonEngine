#pragma once

namespace fbxsdk
{
	class FbxManager;
}

const ObjectClass cSceneNode_FBXNode = UINT16_MAX - 101u;
ZN_INTERFACE ZN_API IFBXSceneNode3D
{
	virtual ~IFBXSceneNode3D() {}

	virtual void InitializeFromFile(const std::string& FileName) = 0;
	virtual std::shared_ptr<IModel> GetModel() const = 0;
};

ZN_INTERFACE ZN_API IFBXManager
{
#ifdef ZN_FBX_SDK_ENABLE
	virtual fbxsdk::FbxManager* GetFBXManager() const = 0;
#endif
};
