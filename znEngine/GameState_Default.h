#pragma once

class ZN_API CGameState_World : public CGameState
{
public:
	CGameState_World(IBaseManager * BaseManager, std::shared_ptr<IRenderWindow> RenderWindow, IWindowEvents* WindowEvents);
	virtual ~CGameState_World();

	// IGameState
	bool Init();
	void Destroy();

	virtual void OnRayIntersected(const glm::vec3& Point) override;

	virtual void OnResize(ResizeEventArgs& e) override;

	virtual void OnPreRender(RenderEventArgs& e) override;
	virtual void OnRender(RenderEventArgs& e) override;
	virtual void OnPostRender(RenderEventArgs& e) override;
	virtual void OnRenderUI(RenderEventArgs& e) override;

	// Keyboard events
	virtual void OnKeyPressed(KeyEventArgs& e);
	virtual void OnKeyReleased(KeyEventArgs& e);

private:
	void Load3D();
	void LoadUI();
	void LoadObj(const std::string& ObjFilename);

private:
	std::shared_ptr<IRenderPass> m_FBX_Opaque_Pass;
	std::shared_ptr<IRenderPass> m_FBX_Transperent_Pass;
};