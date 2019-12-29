#include "stdafx.h"

// General
#include "FBX.h"

// Additional
#include "FBXManager.h"
#include "FBXScene.h"

#include "FBXDisplayMesh.h"

bool DisplayContentNode(CFBX * FBX, FbxNode* pNode);

void DisplayContentScene(CFBX * FBX, FbxScene* pScene)
{
	int i;
	FbxNode* lNode = pScene->GetRootNode();

	if (lNode)
	{
		for (i = 0; i < lNode->GetChildCount(); i++)
		{
			if (DisplayContentNode(FBX, lNode->GetChild(i)))
				return;
		}
	}
}

bool DisplayContentNode(CFBX * FBX, FbxNode* pNode)
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
			std::shared_ptr<IMesh> m = DisplayMesh(FBX, pNode);

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
		DisplayContentNode(FBX, pNode->GetChild(i));
	}

	return false;
}

CFBX::CFBX(const std::string& SceneName, std::shared_ptr<ISceneNode> ParentNode)
	: m_Node(ParentNode)
{
	m_BaseManager = std::dynamic_pointer_cast<IBaseManagerHolder>(m_Node->GetScene())->GetBaseManager();
	
	CFBXManager fbxManager(m_BaseManager);

	std::shared_ptr<CFBXScene> fbxScene = fbxManager.CreateScene("SomeSceneName");
	fbxScene->LoadFromFile(m_BaseManager->GetManager<IFilesManager>()->Open(SceneName));
	fbxScene->Load();


	for (const auto& it : fbxScene->GetMeshes())
		m_Node->GetComponent<IMeshComponent3D>()->AddMesh(it);

	//std::shared_ptr<IFile> savedFile = std::make_shared<CFile>("Sponza\\SponzaConv.fbx");
	//fbxScene->SaveToFile(savedFile);
	//m_BaseManager->GetManager<IFilesManager>()->GetFilesStorage("ZenonGamedata")->SaveFile(savedFile);

	//DisplayContentScene(this, fbxScene->GetNativeScene());
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


