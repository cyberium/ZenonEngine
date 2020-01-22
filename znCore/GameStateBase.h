#pragma once

#include "Passes/RenderTechnique.h"

class ZN_API CGameState 
	: public IGameState
	, public Object
{
public:
	CGameState(IBaseManager * BaseManager, IRenderWindow* RenderWindow, IWindowEvents* WindowEvents);
	virtual ~CGameState();

	// IGameState
	virtual bool                                    Init() override;
	virtual void                                    Destroy() override;

    virtual bool                                    Set() override;
    virtual void                                    Unset() override;

	void											SetInited(bool _value) override;
	bool											IsInited() const override;

	void											SetCurrent(bool _value) override;
	bool											IsCurrent() const override;

	std::shared_ptr<IScene>							GetScene3D() const override { return m_Scene3D; };
	std::shared_ptr<IScene>							GetSceneUI() const override { return m_SceneUI; };

	// GameState events
	virtual void                                    OnRayIntersected(const glm::vec3& Point);

    // Engine events
	virtual void                                    OnUpdate(UpdateEventArgs& e);
	virtual void                                    OnPreRender(RenderEventArgs& e);
	virtual void                                    OnRender(RenderEventArgs& e);
	virtual void                                    OnPostRender(RenderEventArgs& e);
	virtual void                                    OnRenderUI(RenderEventArgs& e);

    // Window events
    virtual void                                    OnResize(ResizeEventArgs& e);

	// Keyboard events
	virtual void                                    OnKeyPressed(KeyEventArgs& e);
	virtual void                                    OnKeyReleased(KeyEventArgs& e);
	virtual void                                    OnKeyboardFocus(EventArgs& e);
	virtual void                                    OnKeyboardBlur(EventArgs& e);

	// Mouse events
	virtual void                                    OnMouseMoved(MouseMotionEventArgs& e);
	virtual void                                    OnMouseButtonPressed(MouseButtonEventArgs& e);
	virtual void                                    OnMouseButtonReleased(MouseButtonEventArgs& e);
	virtual void                                    OnMouseWheel(MouseWheelEventArgs& e);
	virtual void                                    OnMouseLeave(EventArgs& e);
	virtual void                                    OnMouseFocus(EventArgs& e);
	virtual void                                    OnMouseBlur(EventArgs& e);

protected:
	IRenderDevice*                                  GetRenderDevice() const;
	IRenderWindow*                                  GetRenderWindow() const;
	IBaseManager*									GetBaseManager() const;

	void                                            SetCameraController(std::shared_ptr<ICameraController> CameraController);
    std::shared_ptr<ICameraController>              GetCameraController() const;


protected:
	IRenderDevice*                                  m_RenderDevice;
	IRenderWindow*                                  m_RenderWindow;
	IWindowEvents*                                  m_WindowEvents;

    IQuery*                                         m_FrameQuery;
	IQuery*                                         m_TestQuery;
    double                                          m_FrameTime;

	std::shared_ptr<ICameraController>              m_DefaultCameraController;

	std::shared_ptr<ISettingGroup>                  m_VideoSettings;

	RenderTechnique                                 m_Technique3D;
	RenderTechnique                                 m_TechniqueUI;
	std::shared_ptr<IScene>                         m_Scene3D;
	std::shared_ptr<IScene>                         m_SceneUI;


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
	ISceneNodeUI*                                   m_CameraPosText;
	ISceneNodeUI*                                   m_CameraRotText;
	ISceneNodeUI*                                   m_CameraRot2Text;
	ISceneNodeUI*                                   m_FPSText;

private:
	bool                                            m_IsInited;
	bool                                            m_IsCurrent;
	IBaseManager*                                   m_BaseManager;
};
