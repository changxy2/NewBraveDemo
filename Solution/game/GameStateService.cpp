#include "game_pch.h"
#include "GameStateService.h"
#include <NiDefaultClickRenderStep.h>
#include <NiScreenFillingRenderView.h>
#include <NiZBufferProperty.h>
#include <ecr/RenderService.h>

EE_IMPLEMENT_CONCRETE_CLASS_INFO(GameStateService);

GameStateService::GameStateService(void)
	:m_pRenderStep(NULL)
{
}

GameStateService::~GameStateService(void)
{
	
}

efd::SyncResult GameStateService::OnPreInit(efd::IDependencyRegistrar* pDependencyRegistrar)
{
	pDependencyRegistrar->AddDependency<ecr::RenderService>();
	 pDependencyRegistrar->AddDependency<efd::MessageService>();
	return efd::SyncResult_Success;
}
efd::AsyncResult GameStateService::OnInit()
{
	// don't try to init until the render surface has been created
	if (!CheckForSurface())
	{
		EE_FAIL("Our dependency on the RenderService should make this impossible!");
		return efd::AsyncResult_Failure;
	}

	AddRenderStep();

	for (StatesByID::iterator iter = m_mapStates.begin();
		iter != m_mapStates.end();
		++iter)
	{
		iter->second->OnInit();
	}
	//m_bAlreadyRunning = true;

	return efd::AsyncResult_Complete;
}
efd::AsyncResult GameStateService::OnTick()
{
	if (m_CurGameState)
	{
		efd::TimeType time = m_pServiceManager->GetServiceManagerTime();
		m_CurGameState->OnTick(time);
	}

	return efd::AsyncResult_Pending;
}
efd::AsyncResult GameStateService::OnShutdown()
{
	if (m_CurGameState)
	{
		if(!m_CurGameState->Leave())
			return efd::AsyncResult_Pending;
	}

	for (StatesByID::iterator iter = m_mapStates.begin();
		iter != m_mapStates.end();
		++iter)
	{
		iter->second->OnShutdown();
	}

	m_mapStates.clear();

	// clean up our render changes
	RemoveRenderStep();

	return efd::AsyncResult_Complete;
}
const char* GameStateService::GetDisplayName() const
{
	return "GameStateService";
}

void GameStateService::AddState(GameStatePtr pGameState)
{
	EE_ASSERT(pGameState);
	if (pGameState)
	{
		GameState::GameStateID stateId = pGameState->GetCurStateID();		
		if (m_mapStates.find(stateId) != m_mapStates.end())
		{
			EE_ASSERT(FALSE && "has exist this gamestate");
			return;
		}
		
		m_mapStates[stateId] = pGameState;
	}	
}

efd::Bool GameStateService::ChangeState(GameState::GameStateID newState)
{
	GameStatePtr pGameState = FindState(newState);
	EE_ASSERT(pGameState);
	if (pGameState)
	{
		if (m_CurGameState)
		{
			if(!m_CurGameState->Leave())
				return false;
		}
		
		m_CurGameState = pGameState;

		return m_CurGameState->Enter();
	}
	
	return false;
}
GameState* GameStateService::FindState(GameState::GameStateID stateID)
{
	StatesByID::iterator ite = m_mapStates.find(stateID);
	if (ite != m_mapStates.end())
	{
		return ite->second;
	}

	return NULL;
}

void GameStateService::AddRenderStep()
{
	EE_ASSERT(!m_pRenderStep);

	// Grab the main render frame
	ecr::RenderService* pRenderService =
		GetServiceManager()->GetSystemServiceAs<ecr::RenderService>();
	EE_ASSERT(pRenderService);
	ecr::RenderSurface* pRenderSurface = pRenderService->GetActiveRenderSurface();
	EE_ASSERT(pRenderSurface);
	NiRenderFrame* pRenderFrame = pRenderSurface->GetRenderFrame();
	EE_ASSERT(pRenderFrame);

	// construct a new blank render step
	m_pRenderStep = EE_NEW NiDefaultClickRenderStep();
	EE_ASSERT(m_pRenderStep);
	//m_pRenderStep->SetName(kOverlayStepName);

	// add our step to the main frame
	pRenderFrame->PrependRenderStep(m_pRenderStep);

	// configure the step to clear the screen
	NiViewRenderClick* pRenderClick = EE_NEW NiViewRenderClick();
	EE_ASSERT(pRenderClick);
	pRenderClick->SetName("Spark Overlay Click");
	m_pRenderStep->AppendRenderClick(pRenderClick);
	NiScreenFillingRenderView* pClearView = NiScreenFillingRenderView::Create();
	EE_ASSERT(pClearView);
	pClearView->SetName("Spark Overlay Clear View");
	pRenderClick->AppendRenderView(pClearView);
	NiMaterialProperty* pMaterial = EE_NEW NiMaterialProperty();
	pMaterial->SetDiffuseColor(NiColor::BLACK);
	pClearView->AttachProperty(pMaterial);
	NiZBufferProperty* pZBuffer = EE_NEW NiZBufferProperty();
	pZBuffer->SetZBufferTest(false);
	pZBuffer->SetZBufferWrite(false);
	pClearView->AttachProperty(pZBuffer);

	// disable the step by default
	m_pRenderStep->SetActive(false);
}

//------------------------------------------------------------------------------------------------
void GameStateService::RemoveRenderStep()
{
	ecr::RenderService* pRenderService =
		GetServiceManager()->GetSystemServiceAs<ecr::RenderService>();

	// the render service may have been shut down before us
	if (!pRenderService)
		return;

	if (!m_pRenderStep)
		return;

	// grab the main render frame
	ecr::RenderSurface* pRenderSurface = pRenderService->GetActiveRenderSurface();
	EE_ASSERT(pRenderSurface);
	if (!pRenderSurface)
		return;

	NiRenderFrame* pRenderFrame = pRenderSurface->GetRenderFrame();
	EE_ASSERT(pRenderFrame);
	if (!pRenderFrame)
		return;

	// perform the remove
	pRenderFrame->RemoveRenderStep(m_pRenderStep);
}

NiDefaultClickRenderStep* GameStateService::DisableMainRender()
{
	ecr::RenderService* pRenderService = m_pServiceManager->GetSystemServiceAs<ecr::RenderService>();
	EE_ASSERT(pRenderService);
	if (!pRenderService)
	{
		return NULL;
	}
	ecr::RenderSurface* pRenderSurface = pRenderService->GetActiveRenderSurface();
	EE_ASSERT(pRenderSurface);
	if (!pRenderSurface)
	{
		return NULL;
	}

	NiRenderStep* pMainRender = pRenderSurface->GetRenderStep();
	EE_ASSERT(pMainRender);
	pMainRender->SetActive(false);

	EE_ASSERT(m_pRenderStep);
	m_pRenderStep->SetActive(true);
	return m_pRenderStep;
}

NiDefaultClickRenderStep* GameStateService::EnableMainRender()
{
	EE_ASSERT(m_pRenderStep);
	if (!m_pRenderStep)
	{
		return NULL;
	}
	m_pRenderStep->SetActive(false);

	ecr::RenderService* pRenderService = m_pServiceManager->GetSystemServiceAs<ecr::RenderService>();
	EE_ASSERT(pRenderService);
	if (!pRenderService)
	{
		return NULL;
	}
	ecr::RenderSurface* pRenderSurface = pRenderService->GetActiveRenderSurface();
	EE_ASSERT(pRenderSurface);
	if (!pRenderSurface)
	{
		return NULL;
	}

	NiRenderStep* pMainRender = pRenderSurface->GetRenderStep();
	EE_ASSERT(pMainRender);
	if (!pMainRender)
	{
		return NULL;
	}
	pMainRender->SetActive(true);	
	return m_pRenderStep;
}

efd::Bool GameStateService::CheckForSurface()
{
	ecr::RenderService* pRenderService =
		GetServiceManager()->GetSystemServiceAs<ecr::RenderService>();
	EE_ASSERT(pRenderService);
	ecr::RenderSurface* pRenderSurface = pRenderService->GetActiveRenderSurface();

	return (pRenderSurface != NULL);
}
