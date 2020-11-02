#include "stdafx.h"

// General
#include "EditedScene.h"

CEditedScene::CEditedScene(IBaseManager & BaseManager, IRenderWindow& RenderWindow)
	: SceneBase(BaseManager, RenderWindow)
{
}

CEditedScene::~CEditedScene()
{
	Log::Info("Edited scene destroyed.");
}

void CEditedScene::Initialize()
{
	SceneBase::Initialize();
}

void CEditedScene::Finalize()
{
	SceneBase::Finalize();
}

void CEditedScene::RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode>& OwnerNode, const std::shared_ptr<ISceneNode>& ChildNode)
{
	auto root = GetRootSceneNode(); // Edited parent
	_ASSERT(root != nullptr);

	auto realParent = root->GetParent(); // Real parent
	_ASSERT(realParent != nullptr);

	auto& realScene = realParent->GetScene();
	auto& realSceneInternal = dynamic_cast<ISceneInternal&>(realScene);
	realSceneInternal.RaiseSceneChangeEvent(SceneChangeType, OwnerNode, ChildNode);
}
