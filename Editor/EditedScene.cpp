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



//
// IScene
//
void CEditedScene::Initialize()
{
	SceneBase::Initialize();
}

void CEditedScene::Finalize()
{
	SceneBase::Finalize();
}
