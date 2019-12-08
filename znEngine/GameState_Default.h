#pragma once

class OW_ENGINE_API CGameState_World : public CGameState
{
public:
	CGameState_World(IBaseManager * BaseManager, std::shared_ptr<IRenderWindow> RenderWindow);
	virtual ~CGameState_World();

	// IGameState
	bool Init();
	void Destroy();

	virtual void OnResize(ResizeEventArgs& e) override;

	virtual void OnPreRender(RenderEventArgs& e) override;
	virtual void OnRender(RenderEventArgs& e) override;
	virtual void OnPostRender(RenderEventArgs& e) override;
	virtual void OnRenderUI(RenderEventArgs& e) override;


private:
	void Load3D();
	void LoadUI();

private:
	//const vec4 g_ClearColor = vec4(0.39f, 0.58f, 0.93f, 1.0f);
	const vec4 g_ClearColor = vec4(0.2f, 0.2f, 0.2f, 0.3f);
};