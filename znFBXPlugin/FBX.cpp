#include "stdafx.h"

// General
#include "FBX.h"

// Additional
#include "FBXHelpers.h"
#include "FBXDisplayHierarchy.h"
#include "FBXDisplayMesh.h"

bool DisplayContentNode(CFBX * FBX, FbxManager* FBXManager, FbxNode* pNode);

void DisplayContentScene(CFBX * FBX, FbxManager* FBXManager, FbxScene* pScene)
{
	int i;
	FbxNode* lNode = pScene->GetRootNode();

	if (lNode)
	{
		for (i = 0; i < lNode->GetChildCount(); i++)
		{
			if (DisplayContentNode(FBX, FBXManager, lNode->GetChild(i)))
				return;
		}
	}
}

bool DisplayContentNode(CFBX * FBX, FbxManager* FBXManager, FbxNode* pNode)
{
	FbxNodeAttribute::EType lAttributeType;
	int i;

	if (pNode->GetNodeAttribute() == NULL)
	{
		Log::Print("NULL Node Attribute\n\n");
	}
	else
	{
		lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

		switch (lAttributeType)
		{
		default:
			break;
		//case FbxNodeAttribute::eMarker:
		//	DisplayMarker(pNode);
		//	break;

		//case FbxNodeAttribute::eSkeleton:
		//	DisplaySkeleton(pNode);
		//	break;

		case FbxNodeAttribute::eMesh:
		{
			std::shared_ptr<IMesh> m = DisplayMesh(FBX, FBXManager, pNode);

			FBX->GetSceneNode()->GetComponent<IMeshComponent3D>()->AddMesh(m);

			return false;
		}
		break;

		//case FbxNodeAttribute::eNurbs:
		//	DisplayNurb(pNode);
		//	break;

		//case FbxNodeAttribute::ePatch:
		//	DisplayPatch(pNode);
		//	break;

		//case FbxNodeAttribute::eCamera:
		//	DisplayCamera(pNode);
		//	break;

		//case FbxNodeAttribute::eLight:
		//	DisplayLight(pNode);
		//	break;

		//case FbxNodeAttribute::eLODGroup:
		//	DisplayLodGroup(pNode);
		//	break;
		}
	}

	//DisplayUserProperties(pNode);
	//DisplayTarget(pNode);
	//DisplayPivotsAndLimits(pNode);
	//DisplayTransformPropagation(pNode);
	//DisplayGeometricTransform(pNode);

	for (i = 0; i < pNode->GetChildCount(); i++)
	{
		DisplayContentNode(FBX, FBXManager, pNode->GetChild(i));
	}

	return false;
}

CFBX::CFBX(const std::string& SceneName, std::shared_ptr<ISceneNode> ParentNode)
	: m_Node(ParentNode)
{
	m_BaseManager = std::dynamic_pointer_cast<IBaseManagerHolder>(m_Node->GetScene())->GetBaseManager();

	FbxManager* lSdkManager = NULL;
	FbxScene* lScene = NULL;
	bool lResult;

	// Prepare the FBX SDK.
	InitializeSdkObjects(lSdkManager, lScene);
	// Load the scene.

	lResult = LoadScene(lSdkManager, lScene, SceneName.c_str());

	FbxGeometryConverter converter(lSdkManager);
	if (!converter.Triangulate(lScene, true))
	{
		Log::Error("Error while triangulate!");
	}

	DisplayMetaData(lScene);
	DisplayHierarchy(lScene);
	DisplayContentScene(this, lSdkManager, lScene);
}

CFBX::~CFBX()
{
}

const IBaseManager * CFBX::GetBaseManager() const
{
	return m_BaseManager;
}

std::shared_ptr<ISceneNode> CFBX::GetSceneNode()
{
	return m_Node;
}


