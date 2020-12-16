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
	void SetModel(std::shared_ptr<IModel> Model) override;
	void SetTexture(std::shared_ptr<ITexture> Texture) override;
	void SetParticleSystem(std::shared_ptr<IParticleSystem> ParticleSystem) override;
	void SetMaterial(std::shared_ptr<IMaterial> Material) override;

	// SceneBase
	void Initialize() override;
	void Finalize() override;

private:
	void Clean();

private: // Rendering
	std::shared_ptr<ISceneNode> m_SceneNode;
	std::shared_ptr<ISceneNode> m_SceneNodeForModelPreview;
	std::shared_ptr<ISceneNode> m_SceneNodeForParticlePreview;
	std::shared_ptr<ISceneNode> m_SceneNodeForMaterialPreview;

	std::shared_ptr<IUIControlCommon> m_TextureNode;
};
