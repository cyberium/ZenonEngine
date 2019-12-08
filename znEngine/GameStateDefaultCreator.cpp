#include "stdafx.h"

// General
#include "GameStateDefaultCreator.h"

// Additional
#include "GameState_Default.h"

CGameStateDefaultCreator::CGameStateDefaultCreator(IBaseManager* BaseManager)
	: m_BaseManager(BaseManager)
{
}

CGameStateDefaultCreator::~CGameStateDefaultCreator()
{
}



//
// IGameStateCreator
//
size_t CGameStateDefaultCreator::GetGameStatesCount() const
{
	return 1;
}

std::string CGameStateDefaultCreator::GetGameStateName(size_t Index) const
{
	return "DefaultGameState";
}

GameStatePriority CGameStateDefaultCreator::GetGameStatePriority(size_t Index) const
{
	return 1;
}

std::shared_ptr<IGameState> CGameStateDefaultCreator::CreateGameState(size_t Index, std::shared_ptr<IRenderWindow> RenderWindow) const
{
	return std::make_shared<CGameState_World>(m_BaseManager, RenderWindow);
}
