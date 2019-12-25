#include "stdafx.h"

// General
#include "GameStatesFactory.h"

// Additional
#include "GameStateDefaultCreator.h"

CGameStateFactory::CGameStateFactory(IBaseManager* BaseManager)
{
	AddGameStateCreator(std::make_shared<CGameStateDefaultCreator>(BaseManager));
}

CGameStateFactory::~CGameStateFactory()
{
}



//
// IGameStatesFactory
//
void CGameStateFactory::AddGameStateCreator(std::shared_ptr<IGameStateCreator> Creator)
{
	for (size_t i = 0; i < Creator->GetGameStatesCount(); i++)
	{
		m_Creators.push_back(Creator);
		Log::Print("CGameStateFactory: GameState '%s' registered.", Creator->GetGameStateName(i).c_str());
	}
}

void CGameStateFactory::RemoveGameStateCreator(std::shared_ptr<IGameStateCreator> Creator)
{
}

std::shared_ptr<IGameState> CGameStateFactory::CreateGameStateWithHighestPriority(std::shared_ptr<IRenderWindow> RenderWindow, IWindowEvents* WindowEvents) const
{
	GameStatePriority maxPriority = 0;
	std::shared_ptr<IGameStateCreator> gameStateCreator;
	size_t gameStateIndex;

	for (const auto& creator : m_Creators)
	{
		for (size_t i = 0; i < creator->GetGameStatesCount(); i++)
		{
			if (creator->GetGameStatePriority(i) > maxPriority)
			{
				maxPriority = creator->GetGameStatePriority(i);
				gameStateCreator = creator;
				gameStateIndex = i;
			}
		}
	}

	if (gameStateCreator != nullptr)
	{
		return gameStateCreator->CreateGameState(gameStateIndex, RenderWindow, WindowEvents);
	}

	throw std::exception("CGameStateFactory::CreateGameStateWithHighestPriority: Unable to create highest priority GameState.");
}

std::shared_ptr<IGameState> CGameStateFactory::CreateGameState(std::string GameStateName, std::shared_ptr<IRenderWindow> RenderWindow, IWindowEvents* WindowEvents) const
{
	for (const auto& creator : m_Creators)
	{
		for (size_t i = 0; i < creator->GetGameStatesCount(); i++)
		{
			if (creator->GetGameStateName(i) == GameStateName)
			{
				return creator->CreateGameState(i, RenderWindow, WindowEvents);
			}
		}
	}

	throw std::exception(("CGameStateFactory::CreateGameState: Unable to create GameState '" + GameStateName + "'.").c_str());
}



//
// IznPluginsEventListener
//
void CGameStateFactory::OnPluginAdded(std::shared_ptr<IznPlugin> Plugin)
{
	if (std::shared_ptr<IGameStateCreator> creator = std::dynamic_pointer_cast<IGameStateCreator>(Plugin))
	{
		AddGameStateCreator(creator);
	}
}

void CGameStateFactory::OnPluginInitialized(std::shared_ptr<IznPlugin> Plugin)
{
}
