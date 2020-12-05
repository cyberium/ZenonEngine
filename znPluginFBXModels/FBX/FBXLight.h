#pragma once

#ifdef ZN_FBX_SDK_ENABLE
#include <fbxsdk.h>
#include "FBXInterfaces.h"

class ZN_API CFBXLight
	: public IFBXLight
{
public:
	CFBXLight(const IBaseManager& BaseManager, const IFBXNode& FBXNode);
	virtual ~CFBXLight();

	void Load(fbxsdk::FbxLight* NativeLight);

	// IFBXLight
	const SGPULight& GetLight() const override;

private:
	SGPULight* m_Light;
	const IBaseManager& m_BaseManager;
	const IFBXNode& m_FBXNode;
};

#endif
