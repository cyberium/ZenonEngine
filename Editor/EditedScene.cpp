#include "stdafx.h"

// General
#include "EditedScene.h"

CEditedScene::CEditedScene(IBaseManager & BaseManager)
	: SceneBase(BaseManager)
{
}

CEditedScene::~CEditedScene()
{
}

void CEditedScene::Initialize()
{
	SceneBase::Initialize();

	auto fileNames = Utils::GetAllFilesInDirectory("C:\\_engine\\ZenonEngine_gamedata\\models", ".znmdl");

	std::vector<std::string> modelsList;
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirt.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiver.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverBanks.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverCorner.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverCornerBank.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverCornerInner.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverCrossing.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverEnd.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverEntrance.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverRocks.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverSide.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverSideCorner.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverT.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverTile.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverWater.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_grass.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathBend.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathBendBank.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathCorner.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathCornerSmall.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathCross.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathEnd.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathEndClosed.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathOpen.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathRocks.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathSide.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathSideOpen.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathSplit.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathStraight.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathTile.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_riverBend.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_riverBendBank.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_riverCorner.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_riverCornerSmall.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_riverCross.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_riverEnd.fbx");

	if (!fileNames.empty())
	{
		auto it = fileNames.begin();
		auto sizeSqrtDouble = glm::sqrt(fileNames.size());
		size_t sizeSqrt = glm::round(sizeSqrtDouble);
		//sizeSqrt = 6;

		std::shared_ptr<ISceneNode3D> sceneNodeParent = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>(ofkSceneNode3D)->CreateSceneNode3D(this, cSceneNode3D, GetRootNode3D());

		for (size_t x = 0; x < sizeSqrt; x++)
		{
			for (size_t y = 0; y < sizeSqrt; y++)
			{
				if (it == fileNames.end())
					continue;

				auto fileName = (*it++);

				Log::Info(fileName.c_str());

				try
				{
					auto name = CFile(fileName + ".znmdl").Name();
					name = name.substr(0, name.find_first_of('.'));

					std::shared_ptr<ISceneNode3D> sceneNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>(ofkSceneNode3D)->CreateSceneNode3D(this, cSceneNode3D, sceneNodeParent);
					sceneNode->SetName(name);
					sceneNode->SetTranslate(500.0f + glm::vec3(float(x) * 40.0f, 0.0f, float(y) * 40.0f));

					if (GetBaseManager().GetManager<IFilesManager>()->IsFileExists(fileName))
					{
						auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
						if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
						{
							loadable->Load(GetBaseManager().GetManager<IFilesManager>()->Open(fileName));
						}

						sceneNode->GetComponent<IModelsComponent3D>()->AddModel(model);
						sceneNode->GetComponent<IColliderComponent3D>()->SetBounds(model->GetBounds());
						continue;
					}
				}
				catch (const CException& e)
				{
					Log::Error(e.MessageCStr());
				}
			}
		}
	}
}

void CEditedScene::Finalize()
{
	SceneBase::Finalize();
}



std::shared_ptr<ISceneNode3D> CEditedScene::CreateNode(const glm::ivec3& Position, int32 type)
{
	auto it = std::find_if(m_Nodes.begin(), m_Nodes.end(), [&Position](const CEditedScene::SNode& Object) -> bool {
		return (Object.X == Position.x) && (Object.Y == Position.y) && (Object.Z == Position.z);
	});

	if (it != m_Nodes.end())
		return it->SceneNode;

	auto node = CreateSceneNode<SceneNode3D>(GetRootNode3D());
	node->SetTranslate(glm::vec3(Position));
	node->SetScale(glm::vec3(25.0f));
	auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
	if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
		loadable->Load(GetBaseManager().GetManager<IFilesManager>()->Open("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_grass.fbx.znmdl"));

	node->GetComponent<IModelsComponent3D>()->AddModel(model);
	node->GetComponent<IColliderComponent3D>()->SetBounds(/*BoundingBox(glm::vec3(-5.0f), glm::vec3(5.0f))*/model->GetBounds());

	return node;
}