#include "stdafx.h"

#ifdef ZN_FBX_SDK_ENABLE

// General
#include "FBXScene.h"

// Additional
#include "FBXStream.h"
#include "FBXDisplayCommon.h"
#include "FBXDisplayLink.h"
#include "FBXDisplayAnimation.h"
#include "FBXNode.h"
#include "FBXSkeleton.h"
#include "FBXAnimations.h"
#include "FBXMaterials.h"

CFBXScene::CFBXScene(const IBaseManager& BaseManager, fbxsdk::FbxManager* FBXManager, const IznLoaderParams* LoaderParams)
	: m_BaseManager(BaseManager)
	, m_NativeScene(nullptr)
	, m_LoaderParams(LoaderParams)
{
	fbxsdk::FbxScene* fbxScene = fbxsdk::FbxScene::Create(FBXManager, "Default FBX scene.");
	if (fbxScene == nullptr)
		throw CException("Unable to create FBX scene.");

	m_NativeScene = fbxScene;
}

CFBXScene::~CFBXScene()
{
}

//------------------------------------------------------------------------------------------------------

void CFBXScene::LoadFromFile(std::shared_ptr<IFile> File)
{
	Log::Green("FBXScene: Loading '%s' file.", File->Path_Name().c_str());

	if (fbxsdk::FbxDocumentInfo* sceneInfo = m_NativeScene->GetSceneInfo())
	{
		Log::Info("FBXScene: Title: '%s'.", sceneInfo->mTitle.Buffer());
		Log::Info("FBXScene: Subject: '%s'.", sceneInfo->mSubject.Buffer());
		Log::Info("FBXScene: Author: '%s'.", sceneInfo->mAuthor.Buffer());
		Log::Info("FBXScene: Keywords: '%s'.", sceneInfo->mKeywords.Buffer());
		Log::Info("FBXScene: Revision: '%s'.", sceneInfo->mRevision.Buffer());
		Log::Info("FBXScene: Comment: '%s'.", sceneInfo->mComment.Buffer());
	}


	fbxsdk::FbxImporter* fbxImporter = fbxsdk::FbxImporter::Create(m_NativeScene->GetFbxManager(), "");

	// Create wrapper for file
	CFBXStream fbxStream(File, m_NativeScene->GetFbxManager()->GetIOPluginRegistry());
	m_Path = File->Path();

	// Initialize the importer by providing a filename.
	int fileFormat = -1;
	bool importStatus = fbxImporter->Initialize(&fbxStream, NULL, fileFormat, m_NativeScene->GetFbxManager()->GetIOSettings());
	
	int fileVersionMajor, fileVersionMinor, fileVersionRevision;
	fbxImporter->GetFileVersion(fileVersionMajor, fileVersionMinor, fileVersionRevision);

	if (false == importStatus)
	{
		FbxString error = fbxImporter->GetStatus().GetErrorString();
		Log::Print("Call to FbxImporter::Initialize() failed.");
		Log::Print("Error returned: %s", error.Buffer());

		if (fbxImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
			Log::Print("FBX file format version for file '%s' is %d.%d.%d\n\n", File->Path_Name().c_str(), fileVersionMajor, fileVersionMinor, fileVersionRevision);
		throw CException("Input failed.");
	}

	if (fbxImporter->IsFBX())
	{
		Log::Print("FBX file format version for file '%s' is %d.%d.%d", File->Path_Name().c_str(), fileVersionMajor, fileVersionMinor, fileVersionRevision);

		// From this point, it is possible to access animation stack information without
		// the expense of loading the entire file.

#if 1
		Log::Print("Animation Stack Information");

		int lAnimStackCount = fbxImporter->GetAnimStackCount();

		Log::Print("    Number of Animation Stacks: %d", lAnimStackCount);
		Log::Print("    Current Animation Stack: '%s'", fbxImporter->GetActiveAnimStackName().Buffer());

		for (int i = 0; i < lAnimStackCount; i++)
		{
			fbxsdk::FbxTakeInfo* lTakeInfo = fbxImporter->GetTakeInfo(i);

			Log::Print("    Animation Stack %d", i);
			Log::Print("         Name: '%s'", lTakeInfo->mName.Buffer());
			Log::Print("         Description: '%s'", lTakeInfo->mDescription.Buffer());

			// Change the value of the import name if the animation stack should be imported under a different name.
			Log::Print("         Import Name: '%s'", lTakeInfo->mImportName.Buffer());

			// Set the value of the import state to false if the animation stack should be not be imported. 
			Log::Print("         Import State: %s", lTakeInfo->mSelect ? "true" : "false");
		}
#endif

		// Set the import states. By default, the import states are always set to true. The code below shows how to change these states.
		m_NativeScene->GetFbxManager()->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL, true);
		m_NativeScene->GetFbxManager()->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE, true);
		m_NativeScene->GetFbxManager()->GetIOSettings()->SetBoolProp(IMP_FBX_LINK, true);
		m_NativeScene->GetFbxManager()->GetIOSettings()->SetBoolProp(IMP_FBX_SHAPE, true);
		m_NativeScene->GetFbxManager()->GetIOSettings()->SetBoolProp(IMP_FBX_GOBO, true);
		m_NativeScene->GetFbxManager()->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION, true);
		m_NativeScene->GetFbxManager()->GetIOSettings()->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}

	if (false == fbxImporter->Import(m_NativeScene))
		throw CException("FBXImporter: Failed to load scenen '%s'. Error: '%s'.", File->Path_Name().c_str(), fbxImporter->GetStatus().GetErrorString());

#if 0
	if (status == false && fbxImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
	{
		Log::Print("Please enter password: ");

		char lPassword[1024];
		lPassword[0] = '\0';

		FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
			scanf("%s", lPassword);
		FBXSDK_CRT_SECURE_NO_WARNING_END

			FbxString lString(lPassword);

		m_FBXManager->GetIOSettings()->SetStringProp(IMP_FBX_PASSWORD, lString);
		m_FBXManager->GetIOSettings()->SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

		status = fbxImporter->Import(pScene);

		if (status == false && fbxImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
		{
			Log::Print("\nPassword is wrong, import aborted.\n");
		}
	}
#endif

	fbxImporter->Destroy();

	fbxsdk::FbxGeometryConverter converter(m_NativeScene->GetFbxManager());

	Timer t;
	if (!converter.Triangulate(fbxsdk::FbxCast<fbxsdk::FbxScene>(m_NativeScene), true))
		throw CException("FBXScene: Error while FbxGeometryConverter::Triangulate.");
	Log::Info("FBXScene: Triangulation finished in '%f' ms.", t.GetElapsedTime());

	//if (!converter.SplitMeshesPerMaterial(fbxsdk::FbxCast<fbxsdk::FbxScene>(m_NativeScene), true))
	//{
	//	Log::Error("CFBXManager: Error while FbxGeometryConverter::SplitMeshesPerMaterial.");
	//	return false;
	//}

	auto skeleton = MakeShared(CFBXSkeleton, m_BaseManager, *this);
	skeleton->Load(m_NativeScene);
	m_Skeleton = skeleton;

	auto animation = MakeShared(CFBXAnimation, m_BaseManager, *this);
	animation->Load(m_NativeScene);
	m_Animation = animation;

	auto fbxMaterials = MakeShared(CFBXMaterials, m_BaseManager, *this);
	fbxMaterials->Load(m_NativeScene);
	m_Materials = fbxMaterials;

	auto root = MakeShared(CFBXNode, m_BaseManager, *this);
	root->LoadNode(m_NativeScene->GetRootNode());
	m_RootNode = root;
}



//
// IFBXScene
//
std::string CFBXScene::GetFBXFilePath() const
{
	return m_Path;
}

std::shared_ptr<IFBXNode> CFBXScene::GetFBXRootNode() const
{
	return m_RootNode;
}

const std::vector<std::shared_ptr<IFBXModel>>& CFBXScene::GetFBXModels() const
{
	return m_Models;
}

std::shared_ptr<IFBXSkeleton> CFBXScene::GetFBXSkeleton() const
{
	return m_Skeleton;
}

std::shared_ptr<IFBXAnimation> CFBXScene::GetFBXAnimation() const
{
	return m_Animation;
}

std::shared_ptr<IFBXMaterials> CFBXScene::GetFBXMaterials() const
{
	return m_Materials;
}

const IznLoaderParams* CFBXScene::GetLoaderParams() const const
{
	return m_LoaderParams;
}

std::shared_ptr<IModel> CFBXScene::MergeModels()
{
	if (m_MergedModel == nullptr)
	{
		IRenderDevice& renderDevice = m_BaseManager.GetApplication().GetRenderDevice();
		auto mergedModel = renderDevice.GetObjectsFactory().CreateModel();

		for (const auto& b : GetFBXSkeleton()->GetBones())
		{
			mergedModel->AddBone(b);
		}

		for (const auto& a : GetFBXAnimation()->GetAnimations())
		{
			mergedModel->AddAnimation(0, a);
		}

		for (const auto& fbxModel : m_Models)
		{
			auto model = fbxModel->GetModel();

			for (const auto& c : model->GetConnections())
			{
				mergedModel->AddConnection(c.Material, c.Geometry, c.GeometryDrawArgs);
			}

			auto bbox = mergedModel->GetBounds();
			bbox.makeUnion(model->GetBounds());
			mergedModel->SetBounds(bbox);
		}

		m_MergedModel = mergedModel;
	}
	return m_MergedModel;
}



//
// IFBXScenePrivate
//
void CFBXScene::AddModel(const std::shared_ptr<IFBXModel>& Model)
{
	m_Models.push_back(Model);
}

#endif
