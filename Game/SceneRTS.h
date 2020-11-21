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

protected:
	void CreateUnitsModels();
	std::shared_ptr<IModel> CreateUnitModel(std::string ModelName, std::string AnimationName);
	void CreateUnit();
	bool OnTowerButtonClicked(const STowerDescription& TowerDesription);

private:
	std::shared_ptr<CUIControlRTSTowersPanel> m_UIControlRTSTowersPanel;
	std::shared_ptr<ISceneNodeRTSPath>        m_RTSUnitsPath;

	struct SRTSWave
	{
		std::shared_ptr<IModel> Model;
		size_t Count;
		size_t IndervalMS;
	};

	std::vector<SRTSWave>  m_RTSWaves;
	size_t                 m_RTSCurrentWave;
	size_t                 m_RTSCurrentWaveUnit;
	float                  m_LastUnitTime;

};