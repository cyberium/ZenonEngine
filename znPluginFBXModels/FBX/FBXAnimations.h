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
	void ProcessNodeRec(fbxsdk::FbxNode* FBXNode, fbxsdk::FbxAnimStack* FBXAnimStack, size_t FBXStackIndex, fbxsdk::FbxAnimLayer* FBXAnimLayer, size_t FBXLayerIndex, const std::shared_ptr<CAnimation>& Animation);
	void ProcessLayerForNode(fbxsdk::FbxNode* FBXNode, fbxsdk::FbxAnimStack* FBXAnimStack, size_t FBXStackIndex, fbxsdk::FbxAnimLayer* FBXAnimLayer, size_t FBXLayerIndex, const std::shared_ptr<CAnimation>& Animation);
	void ProcessLayerForSkeletonNode(fbxsdk::FbxNode* FBXNode, fbxsdk::FbxAnimStack* FBXAnimStack, size_t FBXStackIndex, fbxsdk::FbxAnimLayer* FBXAnimLayer, size_t FBXLayerIndex, const std::shared_ptr<CAnimation>& Animation);

	//void DisplayCurveKeys(fbxsdk::FbxNode* FBXNode, fbxsdk::FbxAnimCurve* pCurve, CznAnimatedValue<float>& valueInt, size_t AnimationIndex);

private:
	std::vector<std::shared_ptr<IAnimation>> m_Animations;

private:
	const IBaseManager& m_BaseManager;
	const IFBXScene& m_FBXScene;
};

#endif
