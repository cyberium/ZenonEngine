#pragma once

const ObjectClass cSceneBase = UINT16_MAX - 300u;

class ZN_API SceneBase 
	: public IScene
	, public ISceneInternal
	, public IRenderWindowEventListener
	, public INativeWindowEventListener
{
public:
	SceneBase(IBaseManager& BaseManager, IRenderWindow& RenderWindow);
	virtual ~SceneBase();

	// IScene
	IBaseManager&                                   GetBaseManager() const override;
	IRenderDevice&                                  GetRenderDevice() const override;
	IRenderWindow&                                  GetRenderWindow() const override;

	void                                            AddEventListener(std::shared_ptr<ISceneEventsListener> Listener) override;
	void                                            RemoveEventListener(std::shared_ptr<ISceneEventsListener> Listener) override;

	virtual void                                    Initialize() override;
	virtual void                                    Finalize() override;

	std::shared_ptr<ISceneNode>					    GetRootNode3D() const override;
	std::shared_ptr<IUIControl>					    GetRootNodeUI() const override;

	void                                            SetRenderer(std::shared_ptr<IRenderer> Renderer) override;
	std::shared_ptr<IRenderer>                      GetRenderer() const override;

	void                                            SetCameraController(std::shared_ptr<ICameraController> CameraController) override;
	std::shared_ptr<ICameraController>              GetCameraController() const override;

	const ISceneFinder&                             GetFinder() const override;

	// Visit funcitonal
	void                                            Accept(IVisitor* visitor) override;
	void											Freeze() override;
	void											Unfreeze() override;
	void                                            AddChild(const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode) override;
	void                                            RemoveChild(const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode) override;


	// ISceneInternal
	void                                            RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode>& OwnerNode, const std::shared_ptr<ISceneNode>& ChildNode) override;


	// IRenderWindowEventListener
	virtual void                                    OnUpdate(UpdateEventArgs& e) override;
	virtual void                                    OnPreRender(RenderEventArgs& e) override;
	virtual void                                    OnRender(RenderEventArgs& e) override;
	virtual void                                    OnPostRender(RenderEventArgs& e) override;
	virtual void                                    OnRenderUI(RenderEventArgs& e) override;


	// INativeWindowEventListener
	// Window events
	virtual void                                    OnWindowInputFocus(EventArgs& Args) override {} // Window gets input focus
	virtual void                                    OnWindowInputBlur(EventArgs& Args) override {}  // Window loses input focus
	virtual void                                    OnWindowMinimize(EventArgs& Args) override {}   // Window is minimized.
	virtual void                                    OnWindowRestore(EventArgs& Args) override {}    // Window is restored.
	virtual void                                    OnWindowResize(ResizeEventArgs& Args) override;
	virtual void                                    OnWindowClose(WindowCloseEventArgs& Args) override {}
	// Keyboard events
	virtual bool                                    OnWindowKeyPressed(KeyEventArgs& e) override;
	virtual void                                    OnWindowKeyReleased(KeyEventArgs& e) override;
	virtual void                                    OnWindowKeyboardFocus(EventArgs& Args) override {}
	virtual void                                    OnWindowKeyboardBlur(EventArgs& Args) override {}
	// Mouse events
	virtual void                                    OnWindowMouseMoved(MouseMotionEventArgs& e) override;
	virtual bool                                    OnWindowMouseButtonPressed(MouseButtonEventArgs& e) override;
	virtual void                                    OnWindowMouseButtonReleased(MouseButtonEventArgs& e) override;
	virtual bool                                    OnWindowMouseWheel(MouseWheelEventArgs& e) override;
	virtual void                                    OnWindowMouseLeave(EventArgs& e) override {}
	virtual void                                    OnWindowMouseFocus(EventArgs& e) override {}
	virtual void                                    OnWindowMouseBlur(EventArgs& e) override {}
	// Mouse in world events
	virtual bool                                    OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld);
	virtual void                                    OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld);
	virtual void                                    OnMouseMoved(const MouseMotionEventArgs & e, const Ray& RayToWorld);

private: // Input events process recursive
	void                                            DoUpdate_Rec(const std::shared_ptr<ISceneNode>& Node, const UpdateEventArgs& e);

	bool                                            DoKeyPressed_Rec(const std::shared_ptr<IUIControl>& Node, KeyEventArgs& e);
	void                                            DoKeyReleased_Rec(const std::shared_ptr<IUIControl>& Node, KeyEventArgs& e);
	void                                            DoMouseMoved_Rec(const std::shared_ptr<IUIControl>& Node, MouseMotionEventArgs& e);
	bool                                            DoMouseButtonPressed_Rec(const std::shared_ptr<IUIControl>& Node, MouseButtonEventArgs& e);
	void                                            DoMouseButtonReleased_Rec(const std::shared_ptr<IUIControl>& Node, MouseButtonEventArgs& e);
	bool                                            DoMouseWheel_Rec(const std::shared_ptr<IUIControl>& Node, MouseWheelEventArgs& e);

protected:
	std::shared_ptr<ISceneNode>                     m_RootNode3D;
	std::shared_ptr<IUIControl>                     m_RootNodeUI;

	std::shared_ptr<IRenderer>                      m_Renderer;

	std::unique_ptr<CSceneFinder>                   m_Finder;

	std::shared_ptr<IQuery>                         m_FrameQuery;
	std::shared_ptr<IQuery>                         m_TestQuery;
	double                                          m_FrameTime;

	std::chrono::time_point<std::chrono::high_resolution_clock> m_Start, m_End;

	std::shared_ptr<ICameraController>              m_CameraController;
	std::shared_ptr<ISettingGroup>                  m_VideoSettings;

	std::shared_ptr<IUIControl>                   m_CameraPosText;
	std::shared_ptr<IUIControl>                   m_CameraRotText;
	std::shared_ptr<IUIControl>                   m_CameraRot2Text;
	std::shared_ptr<IUIControl>                   m_FPSText;

	std::shared_ptr<IUIControl>                   m_StatisticUpdateText;
	std::shared_ptr<IUIControl>                   m_StatisticPreRenderText;
	std::shared_ptr<IUIControl>                   m_StatisticRenderText;
	std::shared_ptr<IUIControl>                   m_StatisticPostRenderText;
	std::shared_ptr<IUIControl>                   m_StatisticRenderUIText;
	std::shared_ptr<IUIControl>                   m_StatisticSummaText;

protected: // Функционал по отложенному добавлению нод
	std::vector<std::pair<std::shared_ptr<ISceneNode>, std::shared_ptr<ISceneNode>>> m_AddChildList;
	std::vector<std::pair<std::shared_ptr<ISceneNode>, std::shared_ptr<ISceneNode>>> m_RemoveChildList;
	std::mutex                                                                           m_ListsAreBusy;
	std::mutex                                                                           m_SceneIsBusy;

	std::vector<std::shared_ptr<ISceneEventsListener>>                                   m_EventListeners;


private: // Quick access
	IBaseManager&                                   m_BaseManager;
	IRenderDevice&                                  m_RenderDevice;
	IRenderWindow&                                  m_RenderWindow;
	std::mutex                                      m_SceneMutex;
	std::atomic_bool                                m_IsFreezed;
};
