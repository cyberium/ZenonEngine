#pragma once

class CEditor3DPreviewScene
	: public SceneBase
{
public:
	CEditor3DPreviewScene(IBaseManager& BaseManager, IRenderWindow& RenderWindow);
	virtual ~CEditor3DPreviewScene();

	void SetModel(const std::shared_ptr<IModel>& Model);

	// SceneBase
	void Initialize() override;
	void Finalize() override;

private: // Rendering
	std::shared_ptr<ISceneNode> m_Node;
};
