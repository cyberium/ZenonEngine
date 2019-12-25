#pragma once

class CGameStateFactory
	: public IGameStatesFactory
	, public IznPluginsEventListener
{
public:
	CGameStateFactory(IBaseManager* BaseManager);
	virtual ~CGameStateFactory();

	// IGameStatesFactory
	void AddGameStateCreator(std::shared_ptr<IGameStateCreator> Creator) override;
	void RemoveGameStateCreator(std::shared_ptr<IGameStateCreator> Creator) override;
	std::shared_ptr<IGameState> CreateGameStateWithHighestPriority(std::shared_ptr<IRenderWindow> RenderWindow, IWindowEvents* WindowEvents) const override;
	std::shared_ptr<IGameState> CreateGameState(std::string GameStateName, std::shared_ptr<IRenderWindow> RenderWindow, IWindowEvents* WindowEvents) const override;

	// IznPluginsEventListener
	void OnPluginAdded(std::shared_ptr<IznPlugin> Plugin) override;
	void OnPluginInitialized(std::shared_ptr<IznPlugin> Plugin) override;

private:
	std::vector<std::shared_ptr<IGameStateCreator>> m_Creators;
};