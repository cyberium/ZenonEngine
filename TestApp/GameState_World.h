#pragma once

class CGameState_World : public CGameState
{
	typedef CGameState base;
public:
	CGameState_World(IApplication * Application);
	virtual ~CGameState_World();

	// IGameState
	bool Init();
	void Destroy();

	virtual void OnPreRender(RenderEventArgs& e);
	virtual void OnRender(RenderEventArgs& e);
	virtual void OnPostRender(RenderEventArgs& e);
	virtual void OnRenderUI(RenderEventArgs& e);


private:
	void Load3D();
	void LoadUI();

private:
	Viewport                             m_Viewport;

	RenderTechnique                      m_3DTechnique;
	RenderTechnique                      m_3DDeferredTechnique;
	RenderTechnique                    m_UITechnique;

	std::shared_ptr<CFreeCameraController>   m_CameraController;

	std::shared_ptr<Query>               m_FrameQuery;
	double                               m_FrameTime;
	std::shared_ptr<Scene3D>             m_3DScene;
	std::shared_ptr<SceneUI>             m_UIScene;

	std::shared_ptr<CLight3D>            m_DirLight;

	std::shared_ptr<CUITextNode>              m_CameraPosText;
	std::shared_ptr<CUITextNode>              m_CameraRotText;

private:
	std::shared_ptr<CGBuffer>            m_GB;
	

private:
	const uint16 c_WindowsWidth = 1280;
	const uint16 c_WindowsHeight = 1024;
	const vec4 g_ClearColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
};