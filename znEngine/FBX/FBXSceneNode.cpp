#include "stdafx.h"

#ifdef ZN_FBX_SDK_ENABLE

// Include
#include "FBXScene.h"

// General
#include "FBXSceneNode.h"

// Additional
#include "FBXDisplayCommon.h"

#include "FBXMesh.h"
#include "FBXLight.h"

#include "Materials/MaterialDebug.h"
#include "Materials/MaterialTextured.h"

CFBXSceneNode::CFBXSceneNode(const IBaseManager& BaseManager, fbxsdk::FbxManager* FBXManager)
	: m_BaseManager(BaseManager)
	, m_FBXManager(FBXManager)
{

}

CFBXSceneNode::~CFBXSceneNode()
{}

void CFBXSceneNode::InitializeFromFile(const std::string & FileName)
{
	auto file = GetBaseManager().GetManager<IFilesManager>()->Open(FileName);
	if (file == nullptr)
		throw CException("FBXSCeneNode: File '%s' not found.", FileName.c_str());

	auto FBXScene = std::make_shared<CFBXScene>(m_BaseManager, m_FBXManager);
	if (!FBXScene->LoadFromFile(file))
		throw CException("FBXSCeneNode: Unable to load '%s'.", FileName.c_str());

	DisplayMetaData(FBXScene->GetNativeScene());
	DisplayHierarchy(FBXScene->GetNativeScene());

	LoadNode(FBXScene->GetNativeScene()->GetRootNode());
}

void CFBXSceneNode::LoadNode(fbxsdk::FbxNode * NativeNode)
{
	//if (m_FBXScene != nullptr)
	//	throw CException("FBXSceneNode: Node '%s' already initialized.", GetName().c_str());

	fbxsdk::FbxAMatrix& lGlobalTransform = NativeNode->EvaluateLocalTransform();
	glm::mat4 globalTransform;
	for (uint32 i = 0; i < 4; i++)
	{
		for (uint32 j = 0; j < 4; j++)
		{
			globalTransform[i][j] = lGlobalTransform[i][j];
		}
	}

#if 0

	// Get the node’s default TRS properties
	fbxsdk::FbxDouble3 lTranslation = NativeNode->LclTranslation.Get();
	Display4DVector("Translation: ", lTranslation, "");

	fbxsdk::FbxDouble3 lRotation = NativeNode->EvaluateLocalRotation();
	Display4DVector("Rotation: ", lRotation, "");

	fbxsdk::FbxDouble3 lScaling = NativeNode->LclScaling.Get();
	Display4DVector("Scaling: ", lScaling, "");
#endif

	//SetRotation(glm::vec3(lRotation[0], lRotation[1], lRotation[2]));

	SetLocalTransform(globalTransform);

	LoadChilds(NativeNode);

	if (NativeNode->GetNodeAttribute() == nullptr)
		return;

	LoadMaterials(NativeNode);

	switch (NativeNode->GetNodeAttribute()->GetAttributeType())
	{
		case fbxsdk::FbxNodeAttribute::EType::eMesh:
		{
			LoadModel(NativeNode);
			if (auto component = GetComponent<IModelsComponent3D>())
				component->AddModel(m_Model);
		}
		break;

		case fbxsdk::FbxNodeAttribute::EType::eLight:
		{
			LoadLight(NativeNode);
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
			Log::Warn("Unknown FBX node type!");
		}
		break;
		default:
		{
			_ASSERT_EXPR(false, "CFBXSceneNode: Unknown FbxNodeAttribute type.");
		}
		break;
	}


}


const std::vector<std::shared_ptr<CFBXMaterial>>& CFBXSceneNode::GetMaterials() const
{
	return m_MaterialsArray;
}

std::shared_ptr<CFBXMaterial> CFBXSceneNode::GetMaterial(int Index) const
{
	return m_MaterialsArray.at(Index);
}

std::shared_ptr<IModel> CFBXSceneNode::GetModel() const
{
	return m_Model;
}



//
// Protected
//
void CFBXSceneNode::LoadChilds(fbxsdk::FbxNode * NativeNode)
{
	for (int i = 0; i < NativeNode->GetChildCount(); i++)
	{
		fbxsdk::FbxNode* fbxNode = NativeNode->GetChild(i);
		_ASSERT(fbxNode != nullptr);

		std::shared_ptr<CFBXSceneNode> childFbxNode = CreateSceneNode<CFBXSceneNode>(m_BaseManager, m_FBXManager);
		if (childFbxNode == nullptr)
		{
			childFbxNode = std::make_shared<CFBXSceneNode>(m_BaseManager, m_FBXManager);
			std::dynamic_pointer_cast<ISceneNode3DInternal>(shared_from_this())->AddChildInternal(childFbxNode);
		}
		childFbxNode->LoadNode(fbxNode);
	}
}

void CFBXSceneNode::LoadMaterials(fbxsdk::FbxNode * NativeNode)
{
	Log::Print("CFBXSceneNode: Materials count '%d'.", NativeNode->GetMaterialCount());
	for (int i = 0; i < NativeNode->GetMaterialCount(); i++)
	{
		std::shared_ptr<CFBXMaterial> znMaterial = std::make_shared<CFBXMaterial>(m_BaseManager, std::dynamic_pointer_cast<CFBXSceneNode>(shared_from_this()));
		znMaterial->Load(NativeNode->GetMaterial(i));
		m_MaterialsArray.push_back(znMaterial);
	}
}

void CFBXSceneNode::LoadModel(fbxsdk::FbxNode * NativeNode)
{
	auto fbxMesh = std::make_shared<CFBXMesh>(m_BaseManager);
	fbxMesh->SetName(NativeNode->GetName());
	fbxMesh->Load(*this, NativeNode->GetMesh());
	m_Model = fbxMesh;

	
}

void CFBXSceneNode::LoadLight(fbxsdk::FbxNode * NativeNode)
{
	std::shared_ptr<CFBXLight> fbxLight = std::make_shared<CFBXLight>(m_BaseManager, std::dynamic_pointer_cast<CFBXSceneNode>(shared_from_this()));
	fbxLight->Load(NativeNode->GetLight());

	//std::shared_ptr<MaterialTextured> matDebug = std::make_shared<MaterialTextured>(m_BaseManager.GetApplication().GetRenderDevice());
	//matDebug->SetTexture(0, m_BaseManager.GetApplication().GetRenderDevice().GetDefaultTexture());

	//auto geometry = m_BaseManager.GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateCone();
	//auto model = m_BaseManager.GetApplication().GetRenderDevice().GetObjectsFactory().CreateModel();
	//model->AddConnection(matDebug, geometry);
	//GetComponent<IModelsComponent3D>()->AddModel(model);
}

#endif
