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

	
}

void CEditedScene::Finalize()
{
	SceneBase::Finalize();
}

void CEditedScene::RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& OwnerNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	auto root = GetRootNode3D();
	if (root == nullptr)
		return;

	auto parent = root->GetParent().lock();
	if (parent == nullptr)
		return;

	root->GetParent().lock()->GetScene()->RaiseSceneChangeEvent(SceneChangeType, OwnerNode, ChildNode);
}



std::shared_ptr<ISceneNode3D> CEditedScene::CreateNode(const glm::ivec3& Position, const std::string& Type)
{
	//auto it = std::find_if(m_Nodes.begin(), m_Nodes.end(), [&Position](const CEditedScene::SNode& Object) -> bool {
	//	return (Object.X == Position.x) && (Object.Y == Position.y) && (Object.Z == Position.z);
	//});

	//if (it != m_Nodes.end())
	//	return it->SceneNode;

	auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>(otSceneNode3D)->CreateSceneNode3D(this, cSceneNode3D, GetRootNode3D());
	node->SetName(Type);
	//node->SetTranslate(glm::vec3(Position));
	//node->SetScale(glm::vec3(25.0f));
	auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
	if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
	{
		auto file = GetBaseManager().GetManager<IFilesManager>()->Open("C:\\_engine\\ZenonEngine_gamedata\\models\\" + Type + ".fbx.znmdl");
		if (file == nullptr)
			throw CException("File not found.");
		loadable->Load(file);
	}

	node->GetComponent<IModelsComponent3D>()->AddModel(model);
	node->GetComponent<IColliderComponent3D>()->SetBounds(/*BoundingBox(glm::vec3(-5.0f), glm::vec3(5.0f))*/model->GetBounds());

	return node;
}