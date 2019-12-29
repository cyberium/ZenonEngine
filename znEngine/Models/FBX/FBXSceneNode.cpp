#include "stdafx.h"

// Include
#include "FBXScene.h"

// General
#include "FBXSceneNode.h"

// Additional
#include "FBX.h"

CFBXSceneNode::CFBXSceneNode(std::shared_ptr<CFBXScene> OwnerScene, fbxsdk::FbxNode * NativeNode)
	: m_OwnerScene(OwnerScene)
	, m_NativeNode(NativeNode)
{
}

CFBXSceneNode::~CFBXSceneNode()
{
}

void CFBXSceneNode::LoadNode(fbxsdk::FbxNode * NativeNode)
{
	switch (NativeNode->GetNodeAttribute()->GetAttributeType())
	{
		case fbxsdk::FbxNodeAttribute::EType::eMesh:
		{

		}
		break;

		case fbxsdk::FbxNodeAttribute::EType::eUnknown:
		case fbxsdk::FbxNodeAttribute::EType::eNull:
		case fbxsdk::FbxNodeAttribute::EType::eMarker:
		case fbxsdk::FbxNodeAttribute::EType::eSkeleton:
		case fbxsdk::FbxNodeAttribute::EType::eNurbs:
		case fbxsdk::FbxNodeAttribute::EType::ePatch:
		case fbxsdk::FbxNodeAttribute::EType::eCamera:
		case fbxsdk::FbxNodeAttribute::EType::eCameraStereo:
		case fbxsdk::FbxNodeAttribute::EType::eCameraSwitcher:
		case fbxsdk::FbxNodeAttribute::EType::eLight:
		case fbxsdk::FbxNodeAttribute::EType::eOpticalReference:
		case fbxsdk::FbxNodeAttribute::EType::eOpticalMarker:
		case fbxsdk::FbxNodeAttribute::EType::eNurbsCurve:
		case fbxsdk::FbxNodeAttribute::EType::eTrimNurbsSurface:
		case fbxsdk::FbxNodeAttribute::EType::eBoundary:
		case fbxsdk::FbxNodeAttribute::EType::eNurbsSurface:
		case fbxsdk::FbxNodeAttribute::EType::eShape:
		case fbxsdk::FbxNodeAttribute::EType::eLODGroup:
		case fbxsdk::FbxNodeAttribute::EType::eSubDiv:
		case fbxsdk::FbxNodeAttribute::EType::eCachedEffect:
		case fbxsdk::FbxNodeAttribute::EType::eLine:
		default:
		{
			_ASSERT_EXPR(false, "FBX: Unknown FbxNodeAttribute type.");
		}
		break;
	}

	//
	// Load childs
	//
	for (int i = 0; i < NativeNode->GetChildCount(); i++)
	{
		fbxsdk::FbxNode* fbxNode = NativeNode->GetChild(i);

		CreateSceneNode<CFBXSceneNode>(m_OwnerScene, fbxNode);
	}
}



//
// ISceneNode
//
void CFBXSceneNode::Initialize()
{
	CFBX fbx(m_SceneFilename, shared_from_this());
}
