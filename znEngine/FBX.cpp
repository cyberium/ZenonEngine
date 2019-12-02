#include "stdafx.h"

// General
#include "FBX.h"

// Additional
#include "Application.h"
#include "Materials/MaterialDebug.h"
#include "Materials/MaterialTextured.h"
#include "FBXHelpers.h"
#include "FBXDisplayHierarchy.h"
#include "FBXDisplayMesh.h"

void DisplayContent(CFBX * FBX, FbxNode* pNode);

void DisplayContent(CFBX * FBX, FbxScene* pScene)
{
	int i;
	FbxNode* lNode = pScene->GetRootNode();

	if (lNode)
	{
		for (i = 0; i < 1/*lNode->GetChildCount()*/; i++)
		{
			DisplayContent(FBX, lNode->GetChild(i));
		}
	}
}

void DisplayContent(CFBX * FBX, FbxNode* pNode)
{
	FbxNodeAttribute::EType lAttributeType;
	int i;

	if (pNode->GetNodeAttribute() == NULL)
	{
		FBXSDK_printf("NULL Node Attribute\n\n");
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
			std::shared_ptr<IMesh> m = DisplayMesh(pNode);
			m->SetMaterial(FBX->GetDefaultMaterial());

			FBX->GetSceneNode()->GetComponent<CMeshComponent3D>()->AddMesh(m);
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
		DisplayContent(FBX, pNode->GetChild(i));
	}
}

CFBX::CFBX(std::shared_ptr<SceneNode3D> ParentNode)
{
	FbxManager* lSdkManager = NULL;
	FbxScene* lScene = NULL;
	bool lResult;

	// Prepare the FBX SDK.
	InitializeSdkObjects(lSdkManager, lScene);
	// Load the scene.

	lResult = LoadScene(lSdkManager, lScene, "D:\\L_FG_Assets_Pack1.0\\Meshes\\Tomb05_c.FBX");

	m_Node = ParentNode->CreateSceneNode<SceneNode3D>();

	std::shared_ptr<MaterialTextured> mat = std::make_shared<MaterialTextured>();
	mat->SetDiffuseColor(vec4(1.0f, 0.0f, 1.0f, 1.0f));
	mat->SetTexture(0, _RenderDevice->CreateTexture2D("default.png"));
	mat->SetWrapper(mat);
	m_DefaultMaterial = mat;

	DisplayMetaData(lScene);
	DisplayHierarchy(lScene);
	DisplayContent(this, lScene);
}

CFBX::~CFBX()
{
}

std::shared_ptr<SceneNode3D> CFBX::GetSceneNode()
{
	return m_Node;
}

std::shared_ptr<IMaterial> CFBX::GetDefaultMaterial()
{
	return m_DefaultMaterial;
}


