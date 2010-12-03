#pragma once
#include "GameState.h"

class GamePlayingState : public GameState
{
	EE_DECLARE_CLASS1(GamePlayingState, game::CLASS_ID_GamePlayingState, GameState);
	EE_DECLARE_CONCRETE_REFCOUNT;

public:
	GamePlayingState(GameStateService* pMasterService);
	virtual ~GamePlayingState(void);

	virtual void OnInit();
	virtual void OnTick(efd::TimeType time);
	virtual void OnShutdown();

	virtual efd::Bool Enter();
	virtual efd::Bool Leave();
};
