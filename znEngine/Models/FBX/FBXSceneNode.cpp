#include "stdafx.h"

// Include
#include "FBXScene.h"

// General
#include "FBXSceneNode.h"


CFBXSceneNode::CFBXSceneNode(const IBaseManager* BaseManager, std::weak_ptr<CFBXScene> OwnerScene, fbxsdk::FbxNode * NativeNode)
	: m_BaseManager(BaseManager)
	, m_OwnerScene(OwnerScene)
	, m_NativeNode(NativeNode)
{
}

CFBXSceneNode::~CFBXSceneNode()
{
}

void CFBXSceneNode::LoadNode()
{
	//
	// Load childs
	//
	for (int i = 0; i < m_NativeNode->GetChildCount(); i++)
	{
		fbxsdk::FbxNode* fbxNode = m_NativeNode->GetChild(i);

		std::shared_ptr<CFBXSceneNode> childFbxNode = CreateSceneNode<CFBXSceneNode>(m_BaseManager, m_OwnerScene, fbxNode);
		childFbxNode->LoadNode();
	}

	LoadMaterials();

	if (m_NativeNode->GetNodeAttribute() == nullptr)
		return;

	switch (m_NativeNode->GetNodeAttribute()->GetAttributeType())
	{
		case fbxsdk::FbxNodeAttribute::EType::eMesh:
		{
			std::shared_ptr<CFBXMesh> fbxMesh = std::make_shared<CFBXMesh>(m_BaseManager, std::dynamic_pointer_cast<CFBXSceneNode>(shared_from_this()), m_NativeNode->GetMesh());
			fbxMesh->Load();
			GetComponent<IMeshComponent3D>()->AddMesh(fbxMesh);
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
		{
			// Do nothing...
		}
		break;
		default:
		{
			_ASSERT_EXPR(false, "CFBXSceneNode: Unknown FbxNodeAttribute type.");
		}
		break;
	}


}



//------------------------------------------------------------------------------------------------------

bool CFBXSceneNode::LoadMaterials()
{
	Log::Print("CFBXSceneNode: Materials count '%d'.", m_NativeNode->GetMaterialCount());
	for (int i = 0; i < m_NativeNode->GetMaterialCount(); i++)
	{
		std::shared_ptr<CFBXMaterial> znMaterial = std::make_shared<CFBXMaterial>(m_BaseManager, std::dynamic_pointer_cast<CFBXSceneNode>(shared_from_this()), m_NativeNode->GetMaterial(i));
		znMaterial->Load();
		m_MaterialsArray.push_back(znMaterial);
	}

	return true;
}

const std::vector<std::shared_ptr<CFBXMaterial>>& CFBXSceneNode::GetMaterials() const
{
	return m_MaterialsArray;
}

std::shared_ptr<CFBXMaterial> CFBXSceneNode::GetMaterial(int Index) const
{
	return m_MaterialsArray.at(Index);
}

std::weak_ptr<CFBXScene> CFBXSceneNode::GetOwnerScene() const
{
	return m_OwnerScene;
}
