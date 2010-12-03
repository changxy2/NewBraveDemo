#include "game_pch.h"
#include "GamePlayingState.h"
#include "GameStateService.h"

EE_IMPLEMENT_CONCRETE_CLASS_INFO(GamePlayingState);

GamePlayingState::GamePlayingState(GameStateService* pMasterService)
	:GameState(pMasterService, PlayingStateID)
{
}

GamePlayingState::~GamePlayingState(void)
{
}

void GamePlayingState::OnInit()
{
	// 逻辑相关数据初始话
}
void GamePlayingState::OnTick(efd::TimeType time)
{
	// 游戏主逻辑
}
void GamePlayingState::OnShutdown()
{
	// 清除逻辑相关数据
}

efd::Bool GamePlayingState::Enter()
{
	return true;
}
efd::Bool GamePlayingState::Leave()
{
	return true;
}
