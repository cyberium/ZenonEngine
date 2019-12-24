#pragma once

#include "MainEditor.h"

class CGameState_Editor 
	: public CGameState
{
public:
	CGameState_Editor(IBaseManager * BaseManager, std::shared_ptr<IRenderWindow> RenderWindow, MainEditor * Editor);
	virtual ~CGameState_Editor();

	// IGameState
	bool Init();
	void Destroy();

	virtual void OnRayIntersected(const glm::vec3& Point) override;

	virtual void OnResize(ResizeEventArgs& e) override;

	virtual void OnPreRender(RenderEventArgs& e) override;
	virtual void OnRender(RenderEventArgs& e) override;
	virtual void OnPostRender(RenderEventArgs& e) override;
	virtual void OnRenderUI(RenderEventArgs& e) override;

private:
	void Load3D();
	void LoadUI();

private:
	MainEditor * m_Editor;
};