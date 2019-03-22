#pragma once

#include "GameState.h"

struct GameStatesNames
{
	enum List
	{
		GAME_STATE_MENU,
		GAME_STATE_WORLD,
		GAME_STATE_CLIENT,

		GAME_STATE_WMO_VIEW,
		GAME_STATE_M2_VIEW,
		GAME_STATE_CHARACTER_VIEW,
		GAME_STATE_CREATURE_VIEW,
		GAME_STATE_CREATE_CHARACTER
	};
};

// Forward BEGIN
struct IManager;
// Forward END

struct 
	__declspec(uuid("03594F17-5474-47F0-9B6A-7842ACDF888A"))
	IGameStateManager : IManager
{
	virtual ~IGameStateManager() = 0 {};

	virtual void AddGameState(GameStatesNames::List _name, std::shared_ptr<IGameState> _gameState) = 0;

	virtual bool SetGameState(GameStatesNames::List _name) = 0;
	virtual bool SetGameState(std::shared_ptr<IGameState> _newGameState) = 0;

	virtual std::shared_ptr<IGameState> GetGameState() = 0;
};