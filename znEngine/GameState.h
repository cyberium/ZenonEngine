#pragma once

#include "3D/Scene3D.h"
#include "3D/Passes/RenderTechnique.h"

#include "UI/UIScene.h"
#include "UI/Passes/RenderUITechnique.h"

class CGameState : public IGameState
{
public:
    CGameState(const IApplication * _application);
	virtual ~CGameState();

	// IGameState
	virtual bool Init() override;
	virtual void Destroy() override;

    virtual bool Set() override;
    virtual void Unset() override;

	void SetInited(bool _value) override { m_IsInited = _value; }
	bool IsInited() const override { return m_IsInited; }

	void SetCurrent(bool _value) override { m_IsCurrent = _value; }
    bool IsCurrent() const override { return m_IsCurrent; }


	// Input events
	virtual void OnKeyPressed(KeyEventArgs& e);
	virtual void OnKeyReleased(KeyEventArgs& e);
	virtual void OnMouseButtonPressed(MouseButtonEventArgs& e);
	virtual void OnMouseButtonReleased(MouseButtonEventArgs& e);
	virtual void OnMouseMoved(MouseMotionEventArgs& e);
	virtual void OnMouseWheel(MouseWheelEventArgs& e);

	// Window events
	virtual void OnResize(ResizeEventArgs& e);

	// Update event
	virtual void OnUpdate(UpdateEventArgs& e);

	// Render events
	virtual void OnPreRender(Render3DEventArgs& e) = 0;
	virtual void OnRender(Render3DEventArgs& e) = 0;
	virtual void OnPostRender(Render3DEventArgs& e) = 0;
	virtual void OnRenderUI(RenderUIEventArgs& e) = 0;

protected:
	void SetCameraController(std::shared_ptr<ICameraController> CameraController);
	void UnsetCameraController();

protected:
	const IApplication*         m_Application;

    std::shared_ptr<Query>               m_FrameQuery;
    double                               m_FrameTime;

	Viewport                    m_Viewport;
	std::shared_ptr<ICameraController> m_DefaultCameraController;

	const CGroupQuality&        m_QualitySettings;
	const CGroupVideo&          m_VideoSettings;

	RenderTechnique                      m_3DTechnique;
	RenderUITechnique                    m_UITechnique;
	std::shared_ptr<Scene3D>             m_3DScene;
	std::shared_ptr<CUIScene>            m_UIScene;

private: // Input events connections
    Delegate<KeyEventArgs>::FunctionDecl OnKeyPressedConnection;
    Delegate<KeyEventArgs>::FunctionDecl OnKeyReleasedConnection;
    Delegate<MouseButtonEventArgs>::FunctionDecl OnMouseButtonPressedConnection;
    Delegate<MouseButtonEventArgs>::FunctionDecl OnMouseButtonReleasedConnection;
    Delegate<MouseMotionEventArgs>::FunctionDecl OnMouseMovedConnection;
    Delegate<MouseWheelEventArgs>::FunctionDecl OnMouseWheelConnection;

private: // Windows events connection
    Delegate<ResizeEventArgs>::FunctionDecl OnResizeConnection;

private: // Update event connection
    Delegate<UpdateEventArgs>::FunctionDecl OnUpdateConnection;

private: // Render events connection
    Delegate<Render3DEventArgs>::FunctionDecl OnPreRenderConnection;
    Delegate<Render3DEventArgs>::FunctionDecl OnRenderConnection;
    Delegate<Render3DEventArgs>::FunctionDecl OnPostRenderConnection;
    Delegate<RenderUIEventArgs>::FunctionDecl OnRenderUIConnection;

private:
	bool                        m_IsInited;
	bool                        m_IsCurrent;
};
