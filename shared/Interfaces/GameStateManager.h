#pragma once

#include "GameState.h"

enum class ZN_API GameStatesNames
{
	GAME_STATE_MENU,
	GAME_STATE_WORLD,
	GAME_STATE_CLIENT
};

// Forward BEGIN
struct IManager;
// Forward END

struct ZN_API
	__declspec(uuid("03594F17-5474-47F0-9B6A-7842ACDF888A"))
	IGameStateManager : IManager
{
	virtual ~IGameStateManager() = 0 {};

	virtual void AddGameState(GameStatesNames _name, std::shared_ptr<IGameState> _gameState) = 0;

	virtual bool SetGameState(GameStatesNames _name) = 0;
	virtual bool SetGameState(std::shared_ptr<IGameState> _newGameState) = 0;

	virtual std::shared_ptr<IGameState> GetGameState() = 0;
};