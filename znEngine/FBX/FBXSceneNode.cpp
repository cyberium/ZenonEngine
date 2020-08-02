#include "stdafx.h"

#ifdef ZN_FBX_SDK_ENABLE

// General
#include "FBXSceneNode.h"

// Additional
#include "FBXDisplayCommon.h"
#include "FBXDisplayAnimation.h"

#include "FBXMaterial.h"
#include "FBXModel.h"
#include "FBXLight.h"

#include "Materials/MaterialDebug.h"
#include "Materials/MaterialTextured.h"

CFBXSceneNode::CFBXSceneNode(const IBaseManager& BaseManager, IFBXScene& FBXScene)
	: m_BaseManager(BaseManager)
	, m_FBXScene(FBXScene)
	, m_Transform(glm::mat4(1.0f))
{

}

CFBXSceneNode::~CFBXSceneNode()
{}



void CFBXSceneNode::LoadNode(fbxsdk::FbxNode * NativeNode)
{
	//if (m_FBXScene != nullptr)
	//	throw CException("FBXSceneNode: Node '%s' already initialized.", GetName().c_str());

	LoadParams(NativeNode);
	LoadMaterials(NativeNode);
	LoadChilds(NativeNode);
	LoadAttributes(NativeNode);
}




//
// IFBXNode
//
const IFBXScene & CFBXSceneNode::GetScene() const
{
	return m_FBXScene;
}

glm::mat4 CFBXSceneNode::GetTransform() const
{
	return m_Transform;
}

glm::mat4 GetParentWorldTransformRec(const IFBXNode* node)
{
	glm::mat4 transform = node->GetTransform();
	auto parent = node->GetParent().lock();
	if (parent)
		transform = GetParentWorldTransformRec(parent.get()) * transform;
	return transform;
}

glm::mat4 CFBXSceneNode::GetParentWorldTransform() const
{
	if (auto parent = GetParent().lock())
		return GetParentWorldTransformRec(parent.get());
	return glm::mat4(1.0f);
}

std::weak_ptr<IFBXNode> CFBXSceneNode::GetParent() const
{
	return m_Parent;
}

const std::vector<std::shared_ptr<IFBXNode>>& CFBXSceneNode::GetChilds() const
{
	return m_Childs;
}

std::shared_ptr<IFBXMaterial> CFBXSceneNode::GetMaterial(int Index) const
{
	return m_MaterialsArray.at(Index);
}

std::shared_ptr<IFBXModel> CFBXSceneNode::GetModel() const
{
	return m_Model;
}

std::shared_ptr<IFBXLight> CFBXSceneNode::GetLight() const
{
	return m_Light;
}

//
// Protected
//
void CFBXSceneNode::LoadParams(fbxsdk::FbxNode * NativeNode)
{
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

	// Get the node�s default TRS properties
	fbxsdk::FbxDouble3 lTranslation = NativeNode->LclTranslation.Get();
	Display4DVector("Translation: ", lTranslation, "");

	fbxsdk::FbxDouble3 lRotation = NativeNode->EvaluateLocalRotation();
	Display4DVector("Rotation: ", lRotation, "");

	fbxsdk::FbxDouble3 lScaling = NativeNode->LclScaling.Get();
	Display4DVector("Scaling: ", lScaling, "");
#endif

	//SetRotation(glm::vec3(lRotation[0], lRotation[1], lRotation[2]));

	//SetLocalTransform(globalTransform);
}

void CFBXSceneNode::LoadChilds(fbxsdk::FbxNode * NativeNode)
{
	for (int i = 0; i < NativeNode->GetChildCount(); i++)
	{
		fbxsdk::FbxNode* fbxNode = NativeNode->GetChild(i);
		_ASSERT(fbxNode != nullptr);

		std::shared_ptr<CFBXSceneNode> childFbxNode = std::make_shared<CFBXSceneNode>(m_BaseManager, m_FBXScene);
		childFbxNode->LoadNode(fbxNode);
		childFbxNode->m_Parent = weak_from_this();
		m_Childs.push_back(childFbxNode);
	}
}

void CFBXSceneNode::LoadMaterials(fbxsdk::FbxNode * NativeNode)
{
	Log::Print("CFBXSceneNode: Materials count '%d'.", NativeNode->GetMaterialCount());
	for (int i = 0; i < NativeNode->GetMaterialCount(); i++)
	{
		std::shared_ptr<CFBXMaterial> znMaterial = std::make_shared<CFBXMaterial>(m_BaseManager, *this);
		znMaterial->Load(NativeNode->GetMaterial(i));
		m_MaterialsArray.push_back(znMaterial);
	}
}

void CFBXSceneNode::LoadAttributes(fbxsdk::FbxNode * NativeNode)
{
	if (NativeNode->GetNodeAttribute() == nullptr)
		return;

	switch (NativeNode->GetNodeAttribute()->GetAttributeType())
	{
		case fbxsdk::FbxNodeAttribute::EType::eMesh:
			LoadModel(NativeNode);
			break;

		case fbxsdk::FbxNodeAttribute::EType::eLight:
			LoadLight(NativeNode);
			break;

		case fbxsdk::FbxNodeAttribute::EType::eUnknown:
		case fbxsdk::FbxNodeAttribute::EType::eNull:
		case fbxsdk::FbxNodeAttribute::EType::eMarker:
		case fbxsdk::FbxNodeAttribute::EType::eSkeleton:
		{
			// Will be processed later
			//LoadSkeleton(NativeNode);
		}
		break;
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

void CFBXSceneNode::LoadModel(fbxsdk::FbxNode * NativeNode)
{
	auto fbxMesh = std::make_shared<CFBXModel>(m_BaseManager, *this);
	fbxMesh->SetName(NativeNode->GetName());
	if (fbxMesh->Load(NativeNode->GetMesh()))
	{
		_ASSERT(m_Model == nullptr);
		m_Model = fbxMesh;
		dynamic_cast<IFBXScenePrivate&>(m_FBXScene).AddModel(m_Model);
	}
}

void CFBXSceneNode::LoadLight(fbxsdk::FbxNode * NativeNode)
{
	std::shared_ptr<CFBXLight> fbxLight = std::make_shared<CFBXLight>(m_BaseManager, *this);
	fbxLight->Load(NativeNode->GetLight());

	_ASSERT(m_Light == nullptr);
	m_Light = fbxLight;

	//std::shared_ptr<MaterialTextured> matDebug = std::make_shared<MaterialTextured>(m_BaseManager.GetApplication().GetRenderDevice());
	//matDebug->SetTexture(0, m_BaseManager.GetApplication().GetRenderDevice().GetDefaultTexture());

	//auto geometry = m_BaseManager.GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateCone();
	//auto model = m_BaseManager.GetApplication().GetRenderDevice().GetObjectsFactory().CreateModel();
	//model->AddConnection(matDebug, geometry);
	//GetComponent<IModelsComponent3D>()->AddModel(model);
}

#endif
