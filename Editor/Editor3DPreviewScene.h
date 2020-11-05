#pragma once

class CEditor3DPreviewScene
	: public SceneBase
	, public IEditor3DPreviewFrame
{
public:
	CEditor3DPreviewScene(IEditor& Editor, IRenderWindow& RenderWindow);
	virtual ~CEditor3DPreviewScene();

	// IEditor3DPreviewFrame
	void SetSceneNode(std::shared_ptr<ISceneNode> SceneNode) override;
	void SetModel(IModelPtr Model) override;

	// SceneBase
	void Initialize() override;
	void Finalize() override;

private: // Rendering
	std::shared_ptr<ISceneNode> m_SceneNode;
	std::shared_ptr<ISceneNode> m_ModelNode;
};
