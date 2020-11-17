#pragma once

#ifdef ZN_FBX_SDK_ENABLE
#include <fbxsdk.h>
#include "FBXInterfaces.h"

class ZN_API CFBXAnimation
	: public IFBXAnimation
{
public:
	CFBXAnimation(const IBaseManager& BaseManager, const IFBXScene& FBXScene);
	virtual ~CFBXAnimation();

	// CFBXAnimation
	void Load(fbxsdk::FbxScene* FBXScene);

	// IFBXAnimation
	const std::vector<std::shared_ptr<IAnimation>>& GetAnimations() const override;

private:
	void DisplayAnimationRec(fbxsdk::FbxNode* FBXNode, fbxsdk::FbxAnimStack* FBXAnimStack, fbxsdk::FbxAnimLayer* FBXAnimLayer, size_t FBXLayerIndex);
	void DisplayChannels(fbxsdk::FbxNode* FBXNode, fbxsdk::FbxAnimStack* FBXAnimStack, fbxsdk::FbxAnimLayer* FBXAnimLayer, size_t FBXLayerIndex);

	void DisplayCurveKeys(fbxsdk::FbxNode* FBXNode, fbxsdk::FbxAnimCurve* pCurve, CznAnimatedValue<float>& valueInt, size_t AnimationIndex);

private:
	std::vector<std::shared_ptr<IAnimation>> m_Animations;

private:
	const IBaseManager& m_BaseManager;
	const IFBXScene& m_FBXScene;
};

#endif
