#include "stdafx.h"

#ifdef ZN_FBX_SDK_ENABLE

// General
#include "FBXNode.h"

// Additional
#include "FBXDisplayCommon.h"
#include "FBXDisplayAnimation.h"

#include "FBXModel.h"
#include "FBXLight.h"

#include "FBXUtils.h"


CFBXNode::CFBXNode(const IBaseManager& BaseManager, IFBXScene& FBXScene)
	: m_BaseManager(BaseManager)
	, m_FBXScene(FBXScene)
	, m_Transform(glm::mat4(1.0f))
{

}

CFBXNode::~CFBXNode()
{}



void CFBXNode::LoadNode(fbxsdk::FbxNode * NativeNode, int pDepth)
{
	LoadParams(NativeNode);
	LoadMaterials(NativeNode);
	LoadChilds(NativeNode);
	LoadAttributes(NativeNode);
}




//
// IFBXNode
//
const IFBXScene & CFBXNode::GetFBXScene() const
{
	return m_FBXScene;
}

glm::mat4 CFBXNode::GetTransform() const
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

glm::mat4 CFBXNode::GetParentWorldTransform() const
{
	if (auto parent = GetParent().lock())
		return GetParentWorldTransformRec(parent.get());
	return glm::mat4(1.0f);
}

std::weak_ptr<IFBXNode> CFBXNode::GetParent() const
{
	return m_Parent;
}

const std::vector<std::shared_ptr<IFBXNode>>& CFBXNode::GetChilds() const
{
	return m_Childs;
}

std::string CFBXNode::GetFBXMaterialNameByIndex(size_t Index) const const
{
	return m_MaterialsMapping.at(Index);
}

std::shared_ptr<IFBXModel> CFBXNode::GetFBXModel() const
{
	return m_Model;
}

std::shared_ptr<IFBXLight> CFBXNode::GetFBXLight() const
{
	return m_Light;
}

//
// Protected
//
void CFBXNode::LoadParams(fbxsdk::FbxNode * NativeNode)
{
	glm::mat4 globalTransform = ToGLMMat4(NativeNode->EvaluateGlobalTransform());

	auto loaderParams = GetFBXScene().GetLoaderParams();
	if (auto loaderFBXParams = dynamic_cast<const CznFBXLoaderParams*>(loaderParams))
	{
		if (loaderFBXParams->ApplyFullTransform)
		{
			m_Transform = globalTransform;
			return;
		}
	}

	glm::vec3 scale = extractScale(globalTransform);
	m_Transform = glm::scale(scale);
}

void CFBXNode::LoadChilds(fbxsdk::FbxNode * NativeNode)
{
	for (int i = 0; i < NativeNode->GetChildCount(); i++)
	{
		fbxsdk::FbxNode* fbxNode = NativeNode->GetChild(i);
		_ASSERT(fbxNode != nullptr);

		std::shared_ptr<CFBXNode> childFbxNode = MakeShared(CFBXNode, m_BaseManager, m_FBXScene);
		childFbxNode->LoadNode(fbxNode);
		childFbxNode->m_Parent = weak_from_this();
		m_Childs.push_back(childFbxNode);
	}
}

void CFBXNode::LoadMaterials(fbxsdk::FbxNode * NativeNode)
{
	for (int i = 0; i < NativeNode->GetMaterialCount(); i++)
		m_MaterialsMapping.push_back(NativeNode->GetMaterial(i)->GetName());
}

void CFBXNode::LoadAttributes(fbxsdk::FbxNode * NativeNode)
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
			_ASSERT_EXPR(false, "CFBXNode: Unknown FbxNodeAttribute type.");
		}
		break;
	}
}

void CFBXNode::LoadModel(fbxsdk::FbxNode * NativeNode)
{
	auto fbxMesh = MakeShared(CFBXModel, m_BaseManager, *this);
	fbxMesh->SetName(NativeNode->GetName());
	if (fbxMesh->Load(NativeNode->GetMesh()))
	{
		_ASSERT(m_Model == nullptr);
		m_Model = fbxMesh;
		dynamic_cast<IFBXScenePrivate&>(m_FBXScene).AddModel(m_Model);
	}
}

void CFBXNode::LoadLight(fbxsdk::FbxNode * NativeNode)
{
	std::shared_ptr<CFBXLight> fbxLight = MakeShared(CFBXLight, m_BaseManager, *this);
	fbxLight->Load(NativeNode->GetLight());

	_ASSERT(m_Light == nullptr);
	m_Light = fbxLight;

	//std::shared_ptr<MaterialTextured> matDebug = MakeShared(MaterialTextured, m_BaseManager.GetApplication().GetRenderDevice());
	//matDebug->SetTexture(0, m_BaseManager.GetApplication().GetRenderDevice().GetDefaultTexture());

	//auto geometry = m_BaseManager.GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateCone();
	//auto model = m_BaseManager.GetApplication().GetRenderDevice().GetObjectsFactory().CreateModel();
	//model->AddConnection(matDebug, geometry);
	//GetComponentT<IModelsComponent3D>()->AddModel(model);
}

#endif
