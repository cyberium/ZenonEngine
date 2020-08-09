#pragma once

class CEditedScene
	: public SceneBase
{
public:
	CEditedScene(IBaseManager& BaseManager);
	virtual ~CEditedScene();

	// SceneBase
	void Initialize() override;
	void Finalize() override;

	void RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& OwnerNode, const std::shared_ptr<ISceneNode3D>& ChildNode) override;

private:
	struct SNode
	{
		int32 X;
		int32 Y;
		int32 Z;
		int32 Type;
		std::shared_ptr<ISceneNode3D> SceneNode;
	};
	std::vector<SNode> m_Nodes;

	std::shared_ptr<ISceneNode3D> m_LightNode;
};