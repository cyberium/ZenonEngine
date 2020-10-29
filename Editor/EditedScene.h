#pragma once

class CEditedScene
	: public SceneBase
{
public:
	CEditedScene(IBaseManager& BaseManager, IRenderWindow& RenderWindow);
	virtual ~CEditedScene();

	// SceneBase
	void Initialize() override;
	void Finalize() override;

	void RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& OwnerNode, const std::shared_ptr<ISceneNode3D>& ChildNode) override;
};