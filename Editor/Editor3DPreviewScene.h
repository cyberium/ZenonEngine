#pragma once

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

private: // Rendering
	std::shared_ptr<ISceneNode3D> m_Node;
	std::shared_ptr<IMaterialModelPass> m_MaterialModelPass;
	std::shared_ptr<IStructuredBuffer>  m_LightsBuffer;
	size_t							    m_LightsCnt;
};
