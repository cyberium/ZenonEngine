#include "stdafx.h"

// General
#include "EditedScene.h"

CEditedScene::CEditedScene(IBaseManager & BaseManager)
	: SceneBase(BaseManager)
{
}

CEditedScene::~CEditedScene()
{
	Log::Info("Scene destroyed.");
}

void CEditedScene::Initialize()
{
	SceneBase::Initialize();
}

void CEditedScene::Finalize()
{
	SceneBase::Finalize();
}

void CEditedScene::RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& OwnerNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	auto root = GetRootNode3D();
	_ASSERT(root != nullptr);

	auto parent = root->GetParent().lock();
	_ASSERT(parent != nullptr);

	auto realScene = parent->GetScene();
	_ASSERT(realScene != nullptr);

	auto realSceneInternal = dynamic_cast<ISceneInternal*>(realScene);
	_ASSERT(realSceneInternal != nullptr);

	realSceneInternal->RaiseSceneChangeEvent(SceneChangeType, OwnerNode, ChildNode);
}
