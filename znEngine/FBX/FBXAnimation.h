#pragma once

#ifdef ZN_FBX_SDK_ENABLE
#include <fbxsdk.h>

#include "SceneFunctional/3D/AnimatedValue.h"

struct Animation
{
	std::string Name;
	size_t FrameStart;
	size_t FrameEnd;
};

class ZN_API CFBXAnimation
	: public IFBXAnimation
{
public:
	CFBXAnimation(const IBaseManager& BaseManager, const IFBXScene& FBXScene);
	virtual ~CFBXAnimation();

	void Load(fbxsdk::FbxScene* FBXScene);

private:
	void DisplayAnimationRec(FbxAnimLayer* pAnimLayer, FbxNode* pNode, size_t AnimationIndex);
	void DisplayChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer, size_t AnimationIndex);
	void DisplayCurveKeys(FbxAnimCurve* pCurve, AnimatedValue<float>& valueInt, size_t AnimationIndex);

private:
	std::unordered_map<std::string, Animation> m_Animations;
	const IBaseManager& m_BaseManager;
	const IFBXScene& m_FBXScene;
};

#endif
