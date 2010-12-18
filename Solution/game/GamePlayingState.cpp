#include "game_pch.h"
#include "GamePlayingState.h"
#include "GameStateService.h"
#include "ui/MyGUIRenderClick.h"

#include <ecr/RenderService.h>
#include <NiDX9Renderer.h>
#include <ecrInput/InputService.h>

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
	ecr::RenderService* pRenderService = m_pMasterService->GetServiceManager()->GetSystemServiceAs<ecr::RenderService>();
	EE_ASSERT(pRenderService);

	ecr::RenderSurface* pRenderSurface = pRenderService->GetActiveRenderSurface();
	EE_ASSERT(pRenderSurface);	

	NiRenderStep* pMainRender = pRenderSurface->GetRenderStep();
	EE_ASSERT(pMainRender);	

	ecrInput::InputServicePtr pInputService = m_pMasterService->GetServiceManager()->GetSystemServiceAs<ecrInput::InputService>();
	EE_ASSERT(pInputService);

	MyGUIRenderClick* pUIClick = NiNew MyGUIRenderClick(pInputService);
	EE_ASSERT(pUIClick);
	
	NiRenderer* pkRenderrer = pRenderService->GetRenderer();
	EE_ASSERT(pkRenderrer);
	NiDX9Renderer * pkDX9Renderer = NiDynamicCast(NiDX9Renderer,pkRenderrer);
	EE_ASSERT(pkDX9Renderer);
	LPDIRECT3DDEVICE9 pkDevice = pkDX9Renderer->GetD3DDevice();
	EE_ASSERT(pkDevice);

	pUIClick->InitUI(pkDevice);

	((NiDefaultClickRenderStep*)pMainRender)->AppendRenderClick(pUIClick);
	return true;
}
efd::Bool GamePlayingState::Leave()
{
	return true;
}
