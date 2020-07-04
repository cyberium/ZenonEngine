#pragma once

#include "EditorInterfaces.h"
#include "EditedScene.h"
#include "SceneNodesSelector.h"
#include "Passes/DrawSelectionPass.h"

class CEditor3DPreviewScene
	: public SceneBase
{
public:
	CEditor3DPreviewScene(IBaseManager& BaseManager);
	virtual ~CEditor3DPreviewScene();

	void SetModel(const std::shared_ptr<IModel>& Model);

	// SceneBase
	void Initialize() override;
	void Finalize() override;

protected:
	void Load3D();
	void LoadUI();

private:
	std::shared_ptr<ISceneNode3D> m_Node;
};
