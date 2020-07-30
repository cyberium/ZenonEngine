#include "stdafx.h"

// General
#include "FBXAnimation.h"

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
	for (int i = 0; i < FBXScene->GetSrcObjectCount<FbxAnimStack>(); i++)
	{
		FbxAnimStack* lAnimStack = FBXScene->GetSrcObject<FbxAnimStack>(i);

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

		Animation a;
		a.Name = lAnimStack->GetName();
		a.FrameStart = start.GetFrameCount(fbxsdk::FbxTime::eFrames30);
		a.FrameEnd = end.GetFrameCount(fbxsdk::FbxTime::eFrames30);
		m_Animations.insert(std::make_pair(a.Name, a));
	}
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
		fbxsdk::FbxAnimCurve* lAnimCurve = nullptr;
		auto j = m_FBXScene.GetSkeleton()->GetSkeleton().GetBoneByName(pNode->GetName());

		lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (lAnimCurve)
			DisplayCurveKeys(lAnimCurve, j.pX, AnimationIndex);
		lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if (lAnimCurve)
			DisplayCurveKeys(lAnimCurve, j.pY, AnimationIndex);
		lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if (lAnimCurve)
			DisplayCurveKeys(lAnimCurve, j.pZ, AnimationIndex);



		lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (lAnimCurve)
			DisplayCurveKeys(lAnimCurve, j.rX, AnimationIndex);
		lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if (lAnimCurve)
			DisplayCurveKeys(lAnimCurve, j.rY, AnimationIndex);
		lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if (lAnimCurve)
			DisplayCurveKeys(lAnimCurve, j.rZ, AnimationIndex);



		lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (lAnimCurve)
			DisplayCurveKeys(lAnimCurve, j.sX, AnimationIndex);
		lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if (lAnimCurve)
			DisplayCurveKeys(lAnimCurve, j.sY, AnimationIndex);
		lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if (lAnimCurve)
			DisplayCurveKeys(lAnimCurve, j.sZ, AnimationIndex);
	}
	catch (const CException& e)
	{
		Log::Error(e.MessageCStr());
	}
}

void CFBXAnimation::DisplayCurveKeys(fbxsdk::FbxAnimCurve* pCurve, AnimatedValue<float>& valueInt, size_t AnimationIndex)
{
	int lKeyCount = pCurve->KeyGetCount();
	for (int lCount = 0; lCount < lKeyCount; lCount++)
	{
		FbxTime lKeyTime = pCurve->KeyGetTime(lCount);
		valueInt.m_Times[AnimationIndex].push_back(lKeyTime.GetFrameCount(fbxsdk::FbxTime::eFrames30));

		float lKeyValue = static_cast<float>(pCurve->KeyGetValue(lCount));
		valueInt.m_Values[AnimationIndex].push_back(lKeyValue);
		valueInt.m_Type = Interpolations::INTERPOLATION_LINEAR;

		/*lOutputString = "            Key Time: ";
		lOutputString += (int)lKeyTime.GetFrameCount(fbxsdk::FbxTime::eFrames30);
		lOutputString += ".... Key Value: ";
		lOutputString += lKeyValue;
		lOutputString += " [ ";
		lOutputString += interpolation[InterpolationFlagToIndex(pCurve->KeyGetInterpolation(lCount))];
		if ((pCurve->KeyGetInterpolation(lCount)&FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant)
		{
			lOutputString += " | ";
			lOutputString += constantMode[ConstantmodeFlagToIndex(pCurve->KeyGetConstantMode(lCount))];
		}
		else if ((pCurve->KeyGetInterpolation(lCount)&fbxsdk::FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic)
		{
			lOutputString += " | ";
			lOutputString += cubicMode[TangentmodeFlagToIndex(pCurve->KeyGetTangentMode(lCount))];
			lOutputString += " | ";
			lOutputString += tangentWVMode[TangentweightFlagToIndex(pCurve->KeyGet(lCount).GetTangentWeightMode())];
			lOutputString += " | ";
			lOutputString += tangentWVMode[TangentVelocityFlagToIndex(pCurve->KeyGet(lCount).GetTangentVelocityMode())];
		}
		lOutputString += " ]";
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);*/
	}
}


