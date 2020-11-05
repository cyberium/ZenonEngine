#pragma once

class ZN_API SceneBase 
	: public IScene
	, public ISceneInternal
	, public ISceneLoadSave
	, public IRenderWindowEventListener
	, public IznNativeWindowEventListener
{
public:
	SceneBase(IBaseManager& BaseManager, IRenderWindow& RenderWindow);
	virtual ~SceneBase();

	// IScene
	IBaseManager&                                   GetBaseManager() const override;
	IRenderDevice&                                  GetRenderDevice() const override;
	IRenderWindow&                                  GetRenderWindow() const override;
	const ISceneFinder&                             GetFinder() const override;

	virtual void                                    Initialize() override;
	virtual void                                    Finalize() override;

	void                                            AddSceneEventsListener(ISceneEventsListener* Listener) override;
	void                                            RemoveSceneEventsListener(ISceneEventsListener* Listener) override;

	std::shared_ptr<ISceneNode>					    GetRootSceneNode() const override;
	std::shared_ptr<IUIControl>					    GetRootUIControl() const override;

	void                                            SetRenderer(std::shared_ptr<IRenderer> Renderer) override;
	std::shared_ptr<IRenderer>                      GetRenderer() const override;

	void                                            SetCameraController(std::shared_ptr<ICameraController> CameraController) override;
	std::shared_ptr<ICameraController>              GetCameraController() const override;

	// Visit funcitonal
	void                                            Accept(IVisitor* visitor) override;

	// ISceneInternal
	void                                            AddChildInternal(const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode) override;
	void                                            RemoveChildInternal(const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode) override;

	// IRenderWindowEventListener
	virtual void                                    OnUpdate(UpdateEventArgs& e) override;
	virtual void                                    OnRender(RenderEventArgs& e) override;


	// IznNativeWindowEventListener
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
	
	// ISceneLoadSave
	void                                            LoadFromFile(const std::string& FileName) override;
	void                                            SaveToFile(const std::string& FileName) const override;
	void                                            ResetScene() override;


protected:
	virtual bool                                    OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld);
	virtual void                                    OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld);
	virtual void                                    OnMouseMoved(const MouseMotionEventArgs & e, const Ray& RayToWorld);
	
	struct SSceneChangeDelayEvent
	{
		ESceneChangeType EventType;
		std::shared_ptr<ISceneNode> Parent;
		std::shared_ptr<ISceneNode> Child;
	};

	void                                            RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode);


private:
	void                                            DoUpdate_Rec(const std::shared_ptr<ISceneNode>& Node, const UpdateEventArgs& e);

	bool                                            DoKeyPressed_Rec(const std::shared_ptr<IUIControl>& Node, KeyEventArgs& e);
	void                                            DoKeyReleased_Rec(const std::shared_ptr<IUIControl>& Node, KeyEventArgs& e);
	void                                            DoMouseMoved_Rec(const std::shared_ptr<IUIControl>& Node, MouseMotionEventArgs& e);
	bool                                            DoMouseButtonPressed_Rec(const std::shared_ptr<IUIControl>& Node, MouseButtonEventArgs& e);
	void                                            DoMouseButtonReleased_Rec(const std::shared_ptr<IUIControl>& Node, MouseButtonEventArgs& e);
	bool                                            DoMouseWheel_Rec(const std::shared_ptr<IUIControl>& Node, MouseWheelEventArgs& e);

private:
	std::shared_ptr<ISceneNode>                     m_RootSceneNode;
	std::shared_ptr<IUIControl>                     m_RootUIControl;

	std::shared_ptr<IRenderer>                      m_Renderer;

	CSceneFinder                                    m_Finder;

	std::shared_ptr<IQuery>                         m_FrameQuery;
	std::shared_ptr<IQuery>                         m_TestQuery;
	double                                          m_FrameTime;

	std::shared_ptr<ICameraController>              m_CameraController;
	std::shared_ptr<ISettingGroup>                  m_VideoSettings;

	std::shared_ptr<IUIControl>                     m_StatisticText;

private: // Функционал по отложенному добавлению нод
	std::mutex m_SceneChangeDelayEventsLock;
	std::vector<SSceneChangeDelayEvent> m_SceneChangeDelayEvents;

	std::mutex m_ChildModifyLock;

	std::vector<ISceneEventsListener*> m_EventListeners;

private: // Quick access
	IBaseManager&                                   m_BaseManager;
	IRenderDevice&                                  m_RenderDevice;
	IRenderWindow&                                  m_RenderWindow;
	std::mutex                                      m_SceneMutex;
};
