#pragma once

#include "SceneFunctional/3D/Scene3D.h"
#include "SceneFunctional/UI/SceneUI.h"
#include "Passes/RenderTechnique.h"

class OW_ENGINE_API CGameState : public IGameState
{
public:
                                                    CGameState(const IApplication * Application);
	virtual                                         ~CGameState();

	// IGameState
	virtual bool                                    Init() override;
	virtual void                                    Destroy() override;

    virtual bool                                    Set() override;
    virtual void                                    Unset() override;

	void SetInited(bool _value) override { m_IsInited = _value; }
	bool IsInited() const override { return m_IsInited; }

	void SetCurrent(bool _value) override { m_IsCurrent = _value; }
    bool IsCurrent() const override { return m_IsCurrent; }

    // Engine events
	virtual void                                    OnUpdate(UpdateEventArgs& e);
	virtual void                                    OnPreRender(RenderEventArgs& e) = 0;
	virtual void                                    OnRender(RenderEventArgs& e) = 0;
	virtual void                                    OnPostRender(RenderEventArgs& e) = 0;
	virtual void                                    OnRenderUI(RenderEventArgs& e) = 0;

    // Window events
    virtual void                                    OnResize(ResizeEventArgs& e);

    // Keyboard events
    virtual void                                    OnKeyPressed(KeyEventArgs& e);
    virtual void                                    OnKeyReleased(KeyEventArgs& e);

    // Mouse events
    virtual void                                    OnMouseButtonPressed(MouseButtonEventArgs& e);
    virtual void                                    OnMouseButtonReleased(MouseButtonEventArgs& e);
    virtual void                                    OnMouseMoved(MouseMotionEventArgs& e);
    virtual void                                    OnMouseWheel(MouseWheelEventArgs& e);

protected:
    const IApplication*                             GetApplication();

	void                                            SetCameraController(std::shared_ptr<ICameraController> CameraController);
	void                                            UnsetCameraController();
    std::shared_ptr<ICameraController>              GetCameraController() const;

protected:
	const IApplication*                             m_Application;

    std::shared_ptr<Query>                          m_FrameQuery;
    double                                          m_FrameTime;

	Viewport                                        m_Viewport;
	std::shared_ptr<ICameraController>              m_DefaultCameraController;

	const CGroupQuality&                            m_QualitySettings;
	const CGroupVideo&                              m_VideoSettings;

	RenderTechnique                                 m_3DTechnique;
	RenderTechnique                                 m_UITechnique;
	std::shared_ptr<Scene3D>                        m_3DScene;
	std::shared_ptr<SceneUI>                        m_UIScene;

private: // Update event connection
    Delegate<UpdateEventArgs>::FunctionDecl         OnUpdateConnection;
    Delegate<RenderEventArgs>::FunctionDecl         OnPreRenderConnection;
    Delegate<RenderEventArgs>::FunctionDecl         OnRenderConnection;
    Delegate<RenderEventArgs>::FunctionDecl         OnPostRenderConnection;
    Delegate<RenderEventArgs>::FunctionDecl         OnRenderUIConnection;

private: // Windows events connection
    Delegate<ResizeEventArgs>::FunctionDecl         OnResizeConnection;

private: // Keyboard events connections
    Delegate<KeyEventArgs>::FunctionDecl            OnKeyPressedConnection;
    Delegate<KeyEventArgs>::FunctionDecl            OnKeyReleasedConnection;

private: // Mouse events connections
    Delegate<MouseButtonEventArgs>::FunctionDecl    OnMouseButtonPressedConnection;
    Delegate<MouseButtonEventArgs>::FunctionDecl    OnMouseButtonReleasedConnection;
    Delegate<MouseMotionEventArgs>::FunctionDecl    OnMouseMovedConnection;
    Delegate<MouseWheelEventArgs>::FunctionDecl     OnMouseWheelConnection;

private:
	bool                                            m_IsInited;
	bool                                            m_IsCurrent;
};
