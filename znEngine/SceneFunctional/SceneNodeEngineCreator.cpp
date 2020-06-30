#include "stdafx.h"

// General
#include "SceneNodeEngineCreator.h"

// Additional
#include "UI/UIText.h"

#ifdef ZN_FBX_SDK_ENABLE
#include "FBX/FBXManager.h"
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")
#endif

CSceneNode3DEngineCreator::CSceneNode3DEngineCreator(IBaseManager& BaseManager)
	: CObjectClassCreator(BaseManager)
{
	AddKey(cSceneNode3D);
	AddKey(cSceneNode_FBXNode);
}

CSceneNode3DEngineCreator::~CSceneNode3DEngineCreator()
{}

//
// IObjectClassCreator
//
std::shared_ptr<IObject> CSceneNode3DEngineCreator::CreateObject(size_t Index, const IObjectCreationArgs* ObjectCreationArgs)
{
	ISceneNode3DCreationArgs* sceneNodeCreationArgs = static_cast<ISceneNode3DCreationArgs*>(const_cast<IObjectCreationArgs*>(ObjectCreationArgs));

	if (Index == 0)
	{
		return sceneNodeCreationArgs->GetScene()->CreateSceneNode<SceneNode3D>(nullptr);
	}
#ifdef ZN_FBX_SDK_ENABLE
	else if (Index == 1)
	{
		CFBXManager fbxManager(GetBaseManager());

		std::shared_ptr<CFBXSceneNode> node = fbxManager.CreateSceneNode(sceneNodeCreationArgs->GetScene(), "SomeSceneName");
		node->InitializeFromFile("Nature Kit (2.1)\\Models\\FBX format\\tree_thin_dark.fbx");
		//m_FBXScene->LoadFromFile(m_BaseManager.GetManager<IFilesManager>()->Open("Bistro_v4\\Bistro v4 Update\\Bistro_v4\\Bistro_Exterior.fbx"));
		//m_FBXScene->LoadFromFile(m_BaseManager.GetManager<IFilesManager>()->Open("Sponza\\sponza.fbx"));
		//m_FBXScene->LoadFromFile(m_BaseManager.GetManager<IFilesManager>()->Open("tower\\tower.fbx"));
		//m_FBXScene->LoadFromFile(GetBaseManager().GetManager<IFilesManager>()->Open("Nature Kit (2.1)\\Models\\FBX format\\tree_thin_dark.fbx"));
		//m_FBXScene->LoadNodes(sceneNodeCreationArgs->GetScene());

		/*std::shared_ptr<IModel> model = m_FBXScene->ExtractModel();
		if (std::shared_ptr<IObjectLoadSave> loadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(model))
		{
			auto localFileStorage = GetBaseManager().GetManager<IFilesManager>()->GetFilesStorage("ZenonGamedata");

			auto file = std::make_shared<CFile>("generatedModels\\model" + std::string(m_FBXScene->GetRootNode()->GetName()) + ".znmdl");

			loadableFromFile->Save(file);
			file->seek(0);

			localFileStorage->SaveFile(file);


			auto model = GetBaseManager().GetApplication().GetRenderDevice().GetObjectsFactory().CreateModel();
			if (std::shared_ptr<IObjectLoadSave> loadableFromFile2 = std::dynamic_pointer_cast<IObjectLoadSave>(model))
			{
				loadableFromFile2->Load(file);
			}
		}*/

		return node;
	}
#endif

	return nullptr;
}

CSceneNodeUIEngineCreator::CSceneNodeUIEngineCreator(IBaseManager & BaseManager)
	: CObjectClassCreator(BaseManager)
{
	AddKey(cSceneNodeUI);
	AddKey(cSceneNodeUI_Text);
}

CSceneNodeUIEngineCreator::~CSceneNodeUIEngineCreator()
{}

//
// IObjectClassCreator
//
std::shared_ptr<IObject> CSceneNodeUIEngineCreator::CreateObject(size_t Index, const IObjectCreationArgs* ObjectCreationArgs)
{
	ISceneNodeUICreationArgs* sceneNodeCreationArgs = static_cast<ISceneNodeUICreationArgs*>(const_cast<IObjectCreationArgs*>(ObjectCreationArgs));

	if (Index == 0)
	{
		return sceneNodeCreationArgs->GetScene()->CreateSceneNodeUI<SceneNodeUI>(nullptr);
	}
	else if (Index == 1)
	{
		return sceneNodeCreationArgs->GetScene()->CreateSceneNodeUI<CUITextNode>(nullptr);
	}

	return nullptr;
}
