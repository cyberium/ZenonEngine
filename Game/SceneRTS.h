#pragma once

class CSceneRTS
	: public SceneBase
{
public:
	CSceneRTS(IBaseManager& BaseManager, IRenderWindow& RenderWindow);
	virtual ~CSceneRTS();

	// IGameState
	void Initialize() override;
	void Finalize() override;

	void OnUpdate(UpdateEventArgs& e) override;

	bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseMoved(const MouseMotionEventArgs & e, const Ray& RayToWorld) override;

	// Keyboard events
	bool OnWindowKeyPressed(KeyEventArgs& e) override;
	void OnWindowKeyReleased(KeyEventArgs& e) override;


private:
	std::shared_ptr<ISceneNodeRTSPath> m_RTSUnitsPath;
};