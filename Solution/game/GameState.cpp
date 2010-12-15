#include "game_pch.h"
#include "GameState.h"

EE_IMPLEMENT_CONCRETE_CLASS_INFO(GameState);

GameState::GameState(GameStateService* pMasterService, GameStateID thisID, GameStateID nextID)
	:m_pMasterService(pMasterService)
	,m_thisStateID(thisID)
	,m_nextStateID(nextID)
{
}

GameState::~GameState(void)
{
}

void GameState::OnInit()
{

}

void GameState::OnTick(efd::TimeType time)
{

}

void GameState::OnShutdown()
{

}

efd::Bool GameState::Enter()
{
	return true;
}

efd::Bool GameState::Leave()
{
	return true;
}
