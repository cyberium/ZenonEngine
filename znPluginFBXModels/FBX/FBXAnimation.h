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

	void Load(fbxsdk::FbxScene* FBXScene);

	// IFBXAnimation
	const std::vector<std::shared_ptr<IAnimation>>& GetAnimations() const override;

private:
	void DisplayAnimationRec(fbxsdk::FbxAnimLayer* pAnimLayer, fbxsdk::FbxNode* pNode, size_t AnimationIndex);
	void DisplayChannels(fbxsdk::FbxNode* pNode, fbxsdk::FbxAnimLayer* pAnimLayer, size_t AnimationIndex);
	void DisplayCurveKeys(fbxsdk::FbxNode* pNode, fbxsdk::FbxAnimCurve* pCurve, CznAnimatedValue<float>& valueInt, size_t AnimationIndex);

private:
	std::vector<std::shared_ptr<IAnimation>> m_Animations;
	const IBaseManager& m_BaseManager;
	const IFBXScene& m_FBXScene;
};

#endif
