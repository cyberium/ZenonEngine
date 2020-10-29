#include "stdafx.h"

// General
#include "FBXAnimation.h"

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

void CFBXAnimation::Load(fbxsdk::FbxScene * FBXScene)
{
	for (int i = 0; i < FBXScene->GetSrcObjectCount<fbxsdk::FbxAnimStack>(); i++)
	{
		fbxsdk::FbxAnimStack* lAnimStack = FBXScene->GetSrcObject<fbxsdk::FbxAnimStack>(i);

		FbxString lOutputString = "Animation Stack Name: ";
		lOutputString += lAnimStack->GetName();
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);

		int nbAnimLayers = lAnimStack->GetMemberCount<fbxsdk::FbxAnimLayer>();
		lOutputString = "   contains ";
		if (nbAnimLayers == 0)
			lOutputString += "no layers";

		if (nbAnimLayers)
		{
			lOutputString += nbAnimLayers;
			lOutputString += " Animation Layer";
			if (nbAnimLayers > 1)
				lOutputString += "s";
		}

		lOutputString += "\n\n";
		FBXSDK_printf(lOutputString);

		for (int l = 0; l < nbAnimLayers; l++)
		{
			fbxsdk::FbxAnimLayer* lAnimLayer = lAnimStack->GetMember<fbxsdk::FbxAnimLayer>(l);

			lOutputString = "AnimLayer ";
			lOutputString += l;
			lOutputString += "\n";
			FBXSDK_printf(lOutputString);

			DisplayAnimationRec(lAnimLayer, FBXScene->GetRootNode(), l);
		}

		fbxsdk::FbxString animstackname = lAnimStack->GetName();
		fbxsdk::FbxTakeInfo* takeinfo = FBXScene->GetTakeInfo(animstackname);
		fbxsdk::FbxTime start = takeinfo->mLocalTimeSpan.GetStart();
		fbxsdk::FbxTime end = takeinfo->mLocalTimeSpan.GetStop();

		SAnimation a;
		a.Name = lAnimStack->GetName();
		a.FrameStart = start.GetFrameCount(fbxsdk::FbxTime::eFrames60);
		a.FrameEnd = end.GetFrameCount(fbxsdk::FbxTime::eFrames60);
		m_Animations.push_back(a);
	}
}

const std::vector<SAnimation>& CFBXAnimation::GetAnimations() const
{
	return m_Animations;
}

void CFBXAnimation::DisplayAnimationRec(fbxsdk::FbxAnimLayer* pAnimLayer, fbxsdk::FbxNode* pNode, size_t AnimationIndex)
{
	int lModelCount;
	FbxString lOutputString;

	lOutputString = "     Node Name: ";
	lOutputString += pNode->GetName();
	lOutputString += "\n\n";
	FBXSDK_printf(lOutputString);

	DisplayChannels(pNode, pAnimLayer, AnimationIndex);
	FBXSDK_printf("\n");

	for (lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
	{
		DisplayAnimationRec(pAnimLayer, pNode->GetChild(lModelCount), AnimationIndex);
	}
}

void CFBXAnimation::DisplayChannels(fbxsdk::FbxNode* pNode, fbxsdk::FbxAnimLayer* pAnimLayer, size_t AnimationIndex)
{
	try
	{
		auto& bone = m_FBXScene.GetFBXSkeleton()->GetSkeletonEditable().GetBoneByNameEditable(pNode->GetName());

		fbxsdk::FbxTimeSpan interval;
		bool bResult = pNode->GetAnimationInterval(interval);
		fbxsdk::FbxTime start = interval.GetStart();
		fbxsdk::FbxTime end = interval.GetStop();
		fbxsdk::FbxLongLong longstart = start.GetFrameCount();
		fbxsdk::FbxLongLong longend = end.GetFrameCount();

		for (unsigned int i = 0; i < longend; i++)
		{
			fbxsdk::FbxTime keyTime;
			keyTime.SetFrame(i);

			fbxsdk::FbxLongLong keyTimeFrames = keyTime.GetFrameCount(fbxsdk::FbxTime::eFrames30);

			bone.mM.m_Type = Interpolations::INTERPOLATION_LINEAR;

			if (AnimationIndex + 1 >= bone.mM.m_Times.size())
				bone.mM.m_Times.resize(AnimationIndex + 1);
			bone.mM.m_Times[AnimationIndex].push_back(keyTimeFrames);

			FbxAMatrix matGlobal = pNode->EvaluateLocalTransform(keyTime);
			glm::mat4 glmMat = ToGLMMat4(matGlobal);

			if (AnimationIndex + 1 >= bone.mM.m_Values.size())
				bone.mM.m_Values.resize(AnimationIndex + 1);
			bone.mM.m_Values[AnimationIndex].push_back(glmMat);
		}

		
		
		fbxsdk::FbxAnimCurve* curve = nullptr;

		if (curve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X))
			DisplayCurveKeys(pNode, curve,bone.pX, AnimationIndex);
		if (curve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y))
			DisplayCurveKeys(pNode, curve,bone.pY, AnimationIndex);
		if (curve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z))
			DisplayCurveKeys(pNode, curve,bone.pZ, AnimationIndex);

		if (curve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X))
			DisplayCurveKeys(pNode, curve,bone.rX, AnimationIndex);
		if (curve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y))
			DisplayCurveKeys(pNode, curve,bone.rY, AnimationIndex);
		if (curve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z))
			DisplayCurveKeys(pNode, curve,bone.rZ, AnimationIndex);

		if (curve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X))
			DisplayCurveKeys(pNode, curve,bone.sX, AnimationIndex);
		if (curve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y))
			DisplayCurveKeys(pNode, curve,bone.sY, AnimationIndex);
		if (curve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z))
			DisplayCurveKeys(pNode, curve,bone.sZ, AnimationIndex);
	}
	catch (const CException& e)
	{
		Log::Error(e.MessageCStr());
	}
}

void CFBXAnimation::DisplayCurveKeys(fbxsdk::FbxNode* pNode, fbxsdk::FbxAnimCurve* pCurve, CznAnimatedValue<float>& valueInt, size_t AnimationIndex)
{
	int keysCount = pCurve->KeyGetCount();
	for (int i = 0; i < keysCount; i++)
	{
		//if (i != 16)
		//	continue;

		fbxsdk::FbxAnimCurveKey key = pCurve->KeyGet(i);
		fbxsdk::FbxTime keyTime = key.GetTime();
		fbxsdk::FbxLongLong keyTimeFrames = keyTime.GetFrameCount(fbxsdk::FbxTime::eFrames30);

		if (AnimationIndex + 1 >= valueInt.m_Times.size())
			valueInt.m_Times.resize(AnimationIndex + 1);
		valueInt.m_Times[AnimationIndex].push_back(keyTimeFrames);

		//fbxsdk::FbxAMatrix globalBindposeInverseMatrix = pNode->EvaluateGlobalTransform(keyObject);
		//glm::mat4 globalBindposeInverseMatrixGLM;
		//for (uint32 i = 0; i < 4; i++)
		//	for (uint32 j = 0; j < 4; j++)
		//		globalBindposeInverseMatrixGLM[i][j] = globalBindposeInverseMatrix[i][j];

		if (AnimationIndex + 1 >= valueInt.m_Values.size())
		{
			valueInt.m_Values.resize(AnimationIndex + 1);
			//valueInt.m_ValuesHermiteIn.resize(AnimationIndex + 1);
			//valueInt.m_ValuesHermiteOut.resize(AnimationIndex + 1);
		}

		//valueInt.m_Values[AnimationIndex].push_back(keyOOO.GetValue());
		valueInt.m_Values[AnimationIndex].push_back(pCurve->EvaluateIndex(i));

		float val0 = key.GetValue();
		float val1 = pCurve->Evaluate(keyTime);
		float val2 = pCurve->EvaluateIndex(i);


		valueInt.m_Type = Interpolations::INTERPOLATION_LINEAR;

		/*
		if ((pCurve->KeyGetInterpolation(key) & fbxsdk::FbxAnimCurveDef::eInterpolationConstant) == fbxsdk::FbxAnimCurveDef::eInterpolationConstant)
		{
			valueInt.m_Type = Interpolations::INTERPOLATION_NONE;
			valueInt.m_Values[AnimationIndex].push_back(pCurve->KeyGet(key).GetValue());
		}
		else if ((pCurve->KeyGetInterpolation(key)&fbxsdk::FbxAnimCurveDef::eInterpolationCubic) == fbxsdk::FbxAnimCurveDef::eInterpolationLinear)
		{
			valueInt.m_Type = Interpolations::INTERPOLATION_LINEAR;
			valueInt.m_Values[AnimationIndex].push_back(pCurve->KeyGet(key).GetValue());
		}
		else if ((pCurve->KeyGetInterpolation(key)&fbxsdk::FbxAnimCurveDef::eInterpolationCubic) == fbxsdk::FbxAnimCurveDef::eInterpolationCubic)
		{
			fbxsdk::FbxAnimCurveDef::ETangentMode tangentMode = pCurve->KeyGetTangentMode(key);
			fbxsdk::FbxAnimCurveDef::EWeightedMode tangentWeight = pCurve->KeyGet(key).GetTangentWeightMode();
			fbxsdk::FbxAnimCurveDef::EVelocityMode tangentVelocity = pCurve->KeyGet(key).GetTangentVelocityMode();


			valueInt.m_Type = Interpolations::INTERPOLATION_LINEAR;
			valueInt.m_Values[AnimationIndex].push_back(pCurve->KeyGet(key).GetValue());
			valueInt.m_ValuesHermiteIn[AnimationIndex].push_back(pCurve->KeyGet(key).GetDataFloat(fbxsdk::FbxAnimCurveDef::EDataIndex::eNextLeftWeight));
			printf("Test2");
		}*/
	}
}
