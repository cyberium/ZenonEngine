#pragma once

#ifdef ZN_FBX_SDK_ENABLE
#include <fbxsdk.h>

#include "SceneFunctional/3D/AnimatedValue.h"
#include "SceneFunctional/AnimatorComponent.h"

class ZN_API CFBXAnimation
	: public IFBXAnimation
{
public:
	CFBXAnimation(const IBaseManager& BaseManager, const IFBXScene& FBXScene);
	virtual ~CFBXAnimation();

	void Load(fbxsdk::FbxScene* FBXScene);

	// IFBXAnimation
	const std::vector<SAnimation>& GetAnimations() const override;

private:
	void DisplayAnimationRec(fbxsdk::FbxAnimLayer* pAnimLayer, FbxNode* pNode, size_t AnimationIndex);
	void DisplayChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer, size_t AnimationIndex);
	void DisplayCurveKeys(FbxNode* pNode, FbxAnimCurve* pCurve, AnimatedValue<glm::mat4>& valueInt, size_t AnimationIndex);

private:
	std::vector<SAnimation> m_Animations;
	const IBaseManager& m_BaseManager;
	const IFBXScene& m_FBXScene;
};

#endif
