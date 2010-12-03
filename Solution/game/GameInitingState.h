// ======================================================================================
// File         : GameInitingState.h
// Author       : Zhang yuzhong 
// Last Change  : 12/3/2010
// Description  : 游戏初始化阶段 启用一个后台线程加载需要的数据文件
//				  
// ======================================================================================
#pragma once
#include "GameState.h"
#include <NiThreadProcedure.h>

class InitGameThreadProc;
class ScreenTextureClick;

class GameInitingState : public GameState
{
	EE_DECLARE_CLASS1(GameInitingState, game::CLASS_ID_GameInitingState, GameState);
	EE_DECLARE_CONCRETE_REFCOUNT;

	friend class InitGameThreadProc;
public:
	GameInitingState(GameStateService* pMasterService
		, GameStateID nextID
		, efd::utf8string displayTexture);
	virtual ~GameInitingState(void);

public:
	virtual void OnInit();
	virtual void OnTick(efd::TimeType time);
	virtual void OnShutdown();

	virtual efd::Bool Enter();
	virtual efd::Bool Leave();

private:
	efd::Thread*								m_pkInitDataThread;	//初始化游戏数据的线程对象
	InitGameThreadProc*						m_pkThreadProc;			//具体的线程处理函数
	efd::Bool								m_bInitFinish;// 初始化完成
	efd::utf8string							m_pstrDisplayTex;// 显示的图片
	ScreenTextureClick*						m_pRenderClick;

public:
	void StartInitDataThread();
	void CleanupInitDataThread();
};

typedef efd::SmartPointer<GameInitingState> GameInitingStatePtr;


class InitGameThreadProc : public NiThreadProcedure
{
public:
	InitGameThreadProc(GameInitingStatePtr pParams );
	virtual unsigned int ThreadProcedure(efd::Thread* pThread);
protected:
	GameInitingStatePtr	m_pGameInit;		// 传递给线程的参数
};