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

private:
	std::shared_ptr<ISceneNode3D> CreateNode(const glm::ivec3& Position, int32 type);

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
};