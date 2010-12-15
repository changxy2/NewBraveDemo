#pragma once
#include <efd\ISystemService.h>
#include "GameState.h"


class NiDefaultClickRenderStep;

class GameStateService : public efd::ISystemService
{
	EE_DECLARE_CLASS1(GameStateService, game::SERVICE_ID_GameStateService, efd::ISystemService);
	EE_DECLARE_CONCRETE_REFCOUNT;

public:
	GameStateService(void);
	virtual ~GameStateService(void);	

protected:
	/// Overridden virtual functions inherit base documentation and are not documented here.
	virtual efd::SyncResult OnPreInit(efd::IDependencyRegistrar* pDependencyRegistrar);
	virtual efd::AsyncResult OnInit();
	virtual efd::AsyncResult OnTick();
	virtual efd::AsyncResult OnShutdown();
	virtual const char* GetDisplayName() const;


private:
	 GameStatePtr m_CurGameState;
	 NiDefaultClickRenderStep* m_pRenderStep;

public:
	typedef efd::map<GameState::GameStateID, GameStatePtr> StatesByID;

	StatesByID m_mapStates;

private:
	 void AddRenderStep();
	 void RemoveRenderStep();

public:	

	void AddState(GameStatePtr pGameState);

	efd::Bool ChangeState(GameState::GameStateID newState);
	GameState* FindState(GameState::GameStateID stateID);
	inline GameState* GetCurrentState() const { return m_CurGameState; }
	efd::Bool CheckForSurface();	
	NiDefaultClickRenderStep* DisableMainRender();
	NiDefaultClickRenderStep* EnableMainRender();
};

typedef efd::SmartPointer<GameStateService> GameStateServicePtr;
