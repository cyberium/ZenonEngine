#pragma once

class CGameStateDefaultCreator
	: public IGameStateCreator
{
public:
	CGameStateDefaultCreator(IBaseManager* BaseManager);
	virtual ~CGameStateDefaultCreator();

	// IGameStateCreator
	size_t                                  GetGameStatesCount() const override;
	std::string                             GetGameStateName(size_t Index) const override;
	GameStatePriority                       GetGameStatePriority(size_t Index) const override;
	std::shared_ptr<IGameState>             CreateGameState(size_t Index, std::shared_ptr<IRenderWindow> RenderWindow) const override;

private:
	IBaseManager* m_BaseManager;
};