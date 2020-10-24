#include "stdafx.h"

#ifdef ZN_FBX_SDK_ENABLE

// General
#include "FBXScene.h"

// Additional
#include "FBXStream.h"
#include "FBXDisplayCommon.h"
#include "FBXSceneNode.h"
#include "FBXSkeleton.h"
#include "FBXAnimation.h"

CFBXScene::CFBXScene(const IBaseManager& BaseManager, fbxsdk::FbxManager* FBXManager, const std::string& TexturePath)
	: m_BaseManager(BaseManager)
	, m_NativeScene(nullptr)
	, m_TexturesPath(TexturePath)
{
	FbxScene* fbxScene = FbxScene::Create(FBXManager, "Default FBX scene.");
	if (fbxScene == nullptr)
		throw CException("FBXManager: Unable to create FBX scene.");

	m_NativeScene = fbxScene;
}

CFBXScene::~CFBXScene()
{
}

//------------------------------------------------------------------------------------------------------

bool CFBXScene::LoadFromFile(std::shared_ptr<IFile> File)
{
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;
	int lFileFormat = -1;
	int i, lAnimStackCount;
	bool lStatus;
	
	// Get the file version number generate by the FBX SDK.
	fbxsdk::FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(m_NativeScene->GetFbxManager(), "");

	// Create wrapper for file
	CFBXStream fbxStream(File, m_NativeScene->GetFbxManager()->GetIOPluginRegistry());
	m_Path = File->Path();

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(&fbxStream, NULL, lFileFormat, m_NativeScene->GetFbxManager()->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if (!lImportStatus)
	{
		FbxString error = lImporter->GetStatus().GetErrorString();
		Log::Print("Call to FbxImporter::Initialize() failed.");
		Log::Print("Error returned: %s", error.Buffer());

		if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			Log::Print("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			Log::Print("FBX file format version for file '%s' is %d.%d.%d\n\n", File->Path_Name().c_str(), lFileMajor, lFileMinor, lFileRevision);
		}

		return false;
	}

	Log::Print("FBX file format version for this FBX SDK is %d.%d.%d", lSDKMajor, lSDKMinor, lSDKRevision);

	if (lImporter->IsFBX())
	{
		Log::Print("FBX file format version for file '%s' is %d.%d.%d", File->Path_Name().c_str(), lFileMajor, lFileMinor, lFileRevision);

		// From this point, it is possible to access animation stack information without
		// the expense of loading the entire file.

#if 1
		Log::Print("Animation Stack Information");

		lAnimStackCount = lImporter->GetAnimStackCount();

		Log::Print("    Number of Animation Stacks: %d", lAnimStackCount);
		Log::Print("    Current Animation Stack: '%s'", lImporter->GetActiveAnimStackName().Buffer());
		Log::Print("\n");

		for (i = 0; i < lAnimStackCount; i++)
		{
			FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

			Log::Print("    Animation Stack %d", i);
			Log::Print("         Name: '%s'", lTakeInfo->mName.Buffer());
			Log::Print("         Description: '%s'", lTakeInfo->mDescription.Buffer());

			// Change the value of the import name if the animation stack should be imported 
			// under a different name.
			Log::Print("         Import Name: '%s'", lTakeInfo->mImportName.Buffer());

			// Set the value of the import state to false if the animation stack should be not
			// be imported. 
			Log::Print("         Import State: %s", lTakeInfo->mSelect ? "true" : "false");
			Log::Print("\n");
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

	// Import the scene.
	lStatus = lImporter->Import(m_NativeScene);
	if (!lStatus)
	{
		Log::Print("Call to FbxImporter::Import() failed.");
		Log::Print("Error returned: %s", lImporter->GetStatus().GetErrorString());
		return lStatus;
	}

#if 0
	if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
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

		lStatus = lImporter->Import(pScene);

		if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
		{
			Log::Print("\nPassword is wrong, import aborted.\n");
		}
	}
#endif

	lImporter->Destroy();

	FbxGeometryConverter converter(m_NativeScene->GetFbxManager());

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

	auto root = MakeShared(CFBXSceneNode, m_BaseManager, *this);
	root->LoadNode(m_NativeScene->GetRootNode());
	m_RootNode = root;

	return lStatus;
}


//------------------------------------------------------------------------------------------------------
/*
bool CFBXScene::LoadNodes(IScene* Scene)
{
	DisplayMetaData(GetNativeScene());
	DisplayHierarchy(GetNativeScene());

	Log::Print("GEOMETRIES CNT = '%d'", m_NativeScene->GetGeometryCount());
	for (int i = 0; i < m_NativeScene->GetGeometryCount(); i++)
	{
		fbxsdk::FbxGeometry* geom = m_NativeScene->GetGeometry(i);
		Log::Print("%d GEOM NAME = '%s'", i, geom->GetNameOnly().Buffer());
	}

	return true;
}

std::shared_ptr<IModel> CFBXScene::ExtractModel()
{
	m_RootNode = MakeShared(CFBXSceneNode, m_BaseManager, weak_from_this(), m_NativeScene->GetRootNode());
	m_RootNode->LoadNode();

	_ASSERT(m_RootNode->GetChilds().size() == 1);

	auto modelsChild = std::dynamic_pointer_cast<CFBXSceneNode>(m_RootNode->GetChilds().at(0));
	_ASSERT(modelsChild->GetModel() != nullptr);

	return modelsChild->GetModel();
}
*/

//------------------------------------------------------------------------------------------------------

std::string CFBXScene::GetPath() const
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

const std::string& CFBXScene::GetTexturesPath() const
{
	return m_TexturesPath;
}

std::shared_ptr<IModel> CFBXScene::MergeModels()
{
	if (m_MergedModel == nullptr)
	{
		IRenderDevice& renderDevice = m_BaseManager.GetApplication().GetRenderDevice();
		auto mergedModel = renderDevice.GetObjectsFactory().CreateModel();

		for (const auto& m : m_Models)
		{
			auto model = m->GetModel();
			for (const auto& c : model->GetConnections())
			{
				mergedModel->AddConnection(c.Material, c.Geometry, c.GeometryDrawArgs);
			}
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
