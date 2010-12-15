#pragma once
#include <efd\IBase.h>


class GameStateService;
class GameState : public efd::IBase
{
	EE_DECLARE_CLASS1(GameState, game::CLASS_ID_GameState, efd::IBase);
	EE_DECLARE_CONCRETE_REFCOUNT;


public:
	enum GameStateID
	{
		InvalidStateID  =  0,

		InitGameStateID,
		LoadingStateID,
		PlayingStateID,

		FirstUserStateID = 100
	};

	GameState(GameStateService* pMasterService, GameStateID thisID, GameStateID nextID = InvalidStateID);
	virtual ~GameState(void);
	

	GameStateID GetCurStateID() const { return m_thisStateID; }

	virtual void OnInit();
	virtual void OnTick(efd::TimeType time);
	virtual void OnShutdown();

	virtual efd::Bool Leave();
	virtual efd::Bool Enter();

protected:
	GameStateService* m_pMasterService;
	GameStateID m_thisStateID;
	GameStateID m_nextStateID;

};

typedef efd::SmartPointer<GameState> GameStatePtr;
