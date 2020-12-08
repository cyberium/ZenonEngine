#include "stdafx.h"

// General
#include "FBXAnimations.h"

// Additional
#include "FBXUtils.h"

CFBXAnimation::CFBXAnimation(const IBaseManager& BaseManager, const IFBXScene& FBXScene)
	: m_BaseManager(BaseManager)
	, m_FBXScene(FBXScene)
{
}

CFBXAnimation::~CFBXAnimation()
{
}



//
// CFBXAnimation
//
void CFBXAnimation::Load(fbxsdk::FbxScene* FBXScene)
{
	Log::Green("FBXAnimation: Loading...");

	int fbxAnimationsStacksCount = FBXScene->GetSrcObjectCount<fbxsdk::FbxAnimStack>();
	Log::Print("FBXAnimation: Animations stack count: '%d'.", fbxAnimationsStacksCount);

	for (int s = 0; s < fbxAnimationsStacksCount; s++)
	{
		fbxsdk::FbxAnimStack* fbxAnimationStack = FBXScene->GetSrcObject<fbxsdk::FbxAnimStack>(s);
		Log::Print("FBXAnimation:    Process '%s' animation stack.", fbxAnimationStack->GetName());

		int fbxAnimationsLayersCount = fbxAnimationStack->GetMemberCount<fbxsdk::FbxAnimLayer>();
		Log::Print("FBXAnimation:    Animation stack '%s' contains '%d' layers.", fbxAnimationStack->GetName(), fbxAnimationsStacksCount);
		if (fbxAnimationsLayersCount != 1)
			throw CException("Animation stack '%d' contains '%d' layers, but now supported only 1 layer.", s, fbxAnimationsLayersCount);

		fbxsdk::FbxString animationStackName = fbxAnimationStack->GetName();
		fbxsdk::FbxTakeInfo* takeinfo = FBXScene->GetTakeInfo(animationStackName);

		fbxsdk::FbxTime start = takeinfo->mLocalTimeSpan.GetStart();
		fbxsdk::FbxLongLong startAsFrames = start.GetFrameCount(fbxsdk::FbxTime::eFrames60);
		
		fbxsdk::FbxTime end = takeinfo->mLocalTimeSpan.GetStop();
		fbxsdk::FbxLongLong endAsFrames = end.GetFrameCount(fbxsdk::FbxTime::eFrames60);

		std::shared_ptr<CAnimation> animation = MakeShared(CAnimation, s, animationStackName.Buffer(), startAsFrames, endAsFrames);

		for (int l = 0; l < fbxAnimationsLayersCount; l++)
		{
			fbxsdk::FbxAnimLayer* fbxAnimationLayer = fbxAnimationStack->GetMember<fbxsdk::FbxAnimLayer>(l);
			Log::Print("FBXAnimation:       Process '%s' animation layer.", fbxAnimationLayer->GetName());

			ProcessNodeRec(FBXScene->GetRootNode(), fbxAnimationStack, s, fbxAnimationLayer, l, animation);
		}

		m_Animations.push_back(animation);
	}

	Log::Print("FBXAnimation: Loaded '%d' animations.", m_Animations.size());
	Log::Print("");
}

const std::vector<std::shared_ptr<IAnimation>>& CFBXAnimation::GetAnimations() const
{
	return m_Animations;
}

void CFBXAnimation::ProcessNodeRec(fbxsdk::FbxNode* FBXNode, fbxsdk::FbxAnimStack* FBXAnimStack, size_t FBXStackIndex, fbxsdk::FbxAnimLayer* FBXAnimLayer, size_t FBXLayerIndex, const std::shared_ptr<CAnimation>& Animation)
{
	ProcessLayerForNode(FBXNode, FBXAnimStack, FBXStackIndex, FBXAnimLayer, FBXLayerIndex, Animation);

	for (int i = 0; i < FBXNode->GetChildCount(); i++)
		ProcessNodeRec(FBXNode->GetChild(i), FBXAnimStack, FBXStackIndex, FBXAnimLayer, FBXLayerIndex, Animation);
}

void CFBXAnimation::ProcessLayerForNode(fbxsdk::FbxNode* FBXNode, fbxsdk::FbxAnimStack* FBXAnimStack, size_t FBXStackIndex, fbxsdk::FbxAnimLayer* FBXAnimLayer, size_t FBXLayerIndex, const std::shared_ptr<CAnimation>& Animation)
{
	if (FBXNode == nullptr)
	{
		Log::Warn("FBXAnimation:       Node is nullptr.");
		return;
	}

	if (FBXNode->GetNodeAttribute() == nullptr)
	{
		Log::Warn("FBXAnimation:       Node '%s' hasn't attribute.", FBXNode->GetName());
		return;
	}


	if (FBXNode->GetNodeAttribute()->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
	{
		ProcessLayerForSkeletonNode(FBXNode, FBXAnimStack, FBXStackIndex, FBXAnimLayer, FBXLayerIndex, Animation);
	}
	else
	{
		Log::Warn("FBXAnimation:       Node '%s'. Attribute type '%d' is not processed.", FBXNode->GetName(), FBXNode->GetNodeAttribute()->GetAttributeType());
		return;
	}

#if 0
	fbxsdk::FbxAnimCurve* curve = nullptr;

	if (curve = FBXNode->LclTranslation.GetCurve(FBXAnimLayer, FBXSDK_CURVENODE_COMPONENT_X))
		DisplayCurveKeys(FBXNode, curve,skeletonBonePrivateAccess->pX, FBXLayerIndex);
	if (curve = FBXNode->LclTranslation.GetCurve(FBXAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y))
		DisplayCurveKeys(FBXNode, curve,skeletonBonePrivateAccess->pY, FBXLayerIndex);
	if (curve = FBXNode->LclTranslation.GetCurve(FBXAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z))
		DisplayCurveKeys(FBXNode, curve,skeletonBonePrivateAccess->pZ, FBXLayerIndex);

	if (curve = FBXNode->LclRotation.GetCurve(FBXAnimLayer, FBXSDK_CURVENODE_COMPONENT_X))
		DisplayCurveKeys(FBXNode, curve,skeletonBonePrivateAccess->rX, FBXLayerIndex);
	if (curve = FBXNode->LclRotation.GetCurve(FBXAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y))
		DisplayCurveKeys(FBXNode, curve,skeletonBonePrivateAccess->rY, FBXLayerIndex);
	if (curve = FBXNode->LclRotation.GetCurve(FBXAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z))
		DisplayCurveKeys(FBXNode, curve,skeletonBonePrivateAccess->rZ, FBXLayerIndex);

	if (curve = FBXNode->LclScaling.GetCurve(FBXAnimLayer, FBXSDK_CURVENODE_COMPONENT_X))
		DisplayCurveKeys(FBXNode, curve,skeletonBonePrivateAccess->sX, FBXLayerIndex);
	if (curve = FBXNode->LclScaling.GetCurve(FBXAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y))
		DisplayCurveKeys(FBXNode, curve,skeletonBonePrivateAccess->sY, FBXLayerIndex);
	if (curve = FBXNode->LclScaling.GetCurve(FBXAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z))
		DisplayCurveKeys(FBXNode, curve,skeletonBonePrivateAccess->sZ, FBXLayerIndex);
#endif
}

void CFBXAnimation::ProcessLayerForSkeletonNode(fbxsdk::FbxNode * FBXNode, fbxsdk::FbxAnimStack * FBXAnimStack, size_t FBXStackIndex, fbxsdk::FbxAnimLayer * FBXAnimLayer, size_t FBXLayerIndex, const std::shared_ptr<CAnimation>& Animation)
{
	fbxsdk::FbxTimeSpan animationInterval;
	if (false == FBXNode->GetAnimationInterval(animationInterval, FBXAnimStack, FBXLayerIndex))
	{
		Log::Error("FBXAnimation:       Node '%s' hasn't animation interval.", FBXNode->GetName());
		return;
	}

	std::shared_ptr<ISkeletonBone> skeletonBone = nullptr;

	try
	{
		skeletonBone = m_FBXScene.GetFBXSkeleton()->GetBoneByName(FBXNode->GetName());
	}
	catch (const CException& e)
	{
		Log::Warn("FBXAnimation:       Node '%s' hasn't corresponding bone.", FBXNode->GetName());
		return;
	}

	auto skeletonAnimation = Animation->GetSkeletonAnimation();
	if (skeletonAnimation == nullptr)
	{
		skeletonAnimation = MakeShared(CSkeletonAnimation);
		Animation->SetSkeletonAnimation(skeletonAnimation);
	}
	auto skeletonAnimationInternal = std::dynamic_pointer_cast<CSkeletonAnimation>(skeletonAnimation);

	fbxsdk::FbxLongLong animationStartFrame = animationInterval.GetStart().GetFrameCount();
	fbxsdk::FbxLongLong animationEndFrame = animationInterval.GetStop().GetFrameCount();

	CznAnimatedValue<glm::mat4> boneAnimatedValue;
	boneAnimatedValue.m_InterpolationType = Interpolations::Linear;

	for (fbxsdk::FbxLongLong i = animationStartFrame; i <= animationEndFrame; i++)
	{
		fbxsdk::FbxTime keyTime;
		keyTime.SetFrame(i);
		fbxsdk::FbxLongLong keyTimeAsFrames = keyTime.GetFrameCount(fbxsdk::FbxTime::eFrames60);

		// Times
		boneAnimatedValue.m_Times.push_back(keyTimeAsFrames);

		// Value
		boneAnimatedValue.m_Values.push_back(ToGLMMat4(FBXNode->EvaluateLocalTransform(keyTime)));
	}

	if (skeletonBone->GetParentIndex() == -1) // TODO: IsRoot
		skeletonAnimationInternal->SetRootBoneMatrix(skeletonBone->GetLocalMatrix());
	skeletonAnimationInternal->AddBone(skeletonBone->GetName(), boneAnimatedValue);
}

#if 0
void CFBXAnimation::DisplayCurveKeys(fbxsdk::FbxNode* FBXNode, fbxsdk::FbxAnimCurve* pCurve, CznAnimatedValue<float>& valueInt, size_t FBXLayerIndex)
{
	int keysCount = pCurve->KeyGetCount();
	for (int i = 0; i < keysCount; i++)
	{
		//if (i != 16)
		//	continue;

		fbxsdk::FbxAnimCurveKey key = pCurve->KeyGet(i);
		fbxsdk::FbxTime keyTime = key.GetTime();
		fbxsdk::FbxLongLong keyTimeFrames = keyTime.GetFrameCount(fbxsdk::FbxTime::eFrames120);

		if (FBXLayerIndex + 1 >= valueInt.m_Times.size())
			valueInt.m_Times.resize(FBXLayerIndex + 1);
		valueInt.m_Times[FBXLayerIndex].push_back(keyTimeFrames);

		//fbxsdk::FbxAMatrix globalBindposeInverseMatrix = FBXNode->EvaluateGlobalTransform(keyObject);
		//glm::mat4 globalBindposeInverseMatrixGLM;
		//for (uint32 i = 0; i < 4; i++)
		//	for (uint32 j = 0; j < 4; j++)
		//		globalBindposeInverseMatrixGLM[i][j] = globalBindposeInverseMatrix[i][j];

		if (FBXLayerIndex + 1 >= valueInt.m_Values.size())
		{
			valueInt.m_Values.resize(FBXLayerIndex + 1);
			//valueInt.m_ValuesHermiteIn.resize(FBXLayerIndex + 1);
			//valueInt.m_ValuesHermiteOut.resize(FBXLayerIndex + 1);
		}

		//valueInt.m_Values[FBXLayerIndex].push_back(keyOOO.GetValue());
		valueInt.m_Values[FBXLayerIndex].push_back(pCurve->EvaluateIndex(i));

		float val0 = key.GetValue();
		float val1 = pCurve->Evaluate(keyTime);
		float val2 = pCurve->EvaluateIndex(i);


		valueInt.m_Type = Interpolations::INTERPOLATION_LINEAR;

		/*
		if ((pCurve->KeyGetInterpolation(key) & fbxsdk::FbxAnimCurveDef::eInterpolationConstant) == fbxsdk::FbxAnimCurveDef::eInterpolationConstant)
		{
			valueInt.m_Type = Interpolations::INTERPOLATION_NONE;
			valueInt.m_Values[FBXLayerIndex].push_back(pCurve->KeyGet(key).GetValue());
		}
		else if ((pCurve->KeyGetInterpolation(key)&fbxsdk::FbxAnimCurveDef::eInterpolationCubic) == fbxsdk::FbxAnimCurveDef::eInterpolationLinear)
		{
			valueInt.m_Type = Interpolations::INTERPOLATION_LINEAR;
			valueInt.m_Values[FBXLayerIndex].push_back(pCurve->KeyGet(key).GetValue());
		}
		else if ((pCurve->KeyGetInterpolation(key)&fbxsdk::FbxAnimCurveDef::eInterpolationCubic) == fbxsdk::FbxAnimCurveDef::eInterpolationCubic)
		{
			fbxsdk::FbxAnimCurveDef::ETangentMode tangentMode = pCurve->KeyGetTangentMode(key);
			fbxsdk::FbxAnimCurveDef::EWeightedMode tangentWeight = pCurve->KeyGet(key).GetTangentWeightMode();
			fbxsdk::FbxAnimCurveDef::EVelocityMode tangentVelocity = pCurve->KeyGet(key).GetTangentVelocityMode();


			valueInt.m_Type = Interpolations::INTERPOLATION_LINEAR;
			valueInt.m_Values[FBXLayerIndex].push_back(pCurve->KeyGet(key).GetValue());
			valueInt.m_ValuesHermiteIn[FBXLayerIndex].push_back(pCurve->KeyGet(key).GetDataFloat(fbxsdk::FbxAnimCurveDef::EDataIndex::eNextLeftWeight));
			printf("Test2");
		}*/
	}
}
#endif