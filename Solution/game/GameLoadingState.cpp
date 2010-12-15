#include "game_pch.h"
#include "GameLoadingState.h"
#include "GameStateService.h"
#include "GameWorldService.h"

EE_IMPLEMENT_CONCRETE_CLASS_INFO(GameLoadingState);

//EE_HANDLER(GameLoadingState, HandleWorldLoadedMessage, egf::EntityLoadResult);

GameLoadingState::GameLoadingState(GameStateService* pMasterService, GameStateID nextID)
	:GameState(pMasterService, LoadingStateID, nextID)
{
}

GameLoadingState::~GameLoadingState(void)
{
	Cleanup();
}


void GameLoadingState::OnInit()
{

}
void GameLoadingState::OnTick(efd::TimeType time)
{
	//EE_UNUSED_ARG(time);
	if (LoadingIsDone())
	{
		m_pMasterService->ChangeState(m_nextStateID);
	}
	else
	{
		if (!m_blockFiles.empty() && !m_blockFiles.front().m_queued)
		{
			// this is the first one, tell it to load.
			LoadBlockFile(m_blockFiles.front());
		}
		if (!m_pCurrentSplash || m_pCurrentSplash->IsDone())
		{
			// Save the current one before getting the next splash screen because we want the last
			// splash to stay until the transition is complete.
			m_pLastSplash = m_pCurrentSplash;
			m_pCurrentSplash = GetNextSplash();
		}
		// update the current one.
		if (m_pCurrentSplash && m_pCurrentSplash->IsDrawing() && m_pLastSplash)
		{
			// delete our old splash screen now that the new one is rendering.
			m_pRenderStep->RemoveRenderClick(m_pLastSplash);
			m_pLastSplash = 0;
		}
	}
}
void GameLoadingState::OnShutdown()
{

}

efd::Bool GameLoadingState::Enter()
{  
	NiRenderStep* pRenderStep = m_pMasterService->DisableMainRender();
	EE_ASSERT(pRenderStep);

	m_pRenderStep = NiDynamicCast(NiDefaultClickRenderStep, pRenderStep);
	EE_ASSERT(m_pRenderStep);

	AddSplash(EE_NEW LogoSplash("gamebryoLogo.dds", true, LogoSplash::UseTextureSize)); 	

// 	efd::MessageService* pMessageService = m_pMasterService->GetServiceManager()->
// 		GetSystemServiceAs<efd::MessageService>();
// 	EE_ASSERT(pMessageService);
// 	m_myCallbackCat = pMessageService->GetGloballyUniqueCategory();
// 	pMessageService->Subscribe(this, m_myCallbackCat);

	return true;
}
efd::Bool GameLoadingState::Leave()
{
	if (!LoadingIsDone())
	{
		return false;
	}

	// we should not have any remaining splash screens.
	EE_ASSERT(m_splashScreens.empty());

	Cleanup();

// 	efd::MessageService* pMessageService = m_pMasterService->GetServiceManager()->
// 		GetSystemServiceAs<efd::MessageService>();
// 	EE_ASSERT(pMessageService);
// 	pMessageService->Unsubscribe(this, m_myCallbackCat);

	GameWorldServicePtr pGameWorld = m_pMasterService->GetServiceManager()->GetSystemServiceAs<GameWorldService>();
	EE_ASSERT(pGameWorld);
	if (pGameWorld)
	{
		pGameWorld->removeObserver(this);
	}

	m_pMasterService->EnableMainRender();

	return true;
}

void GameLoadingState::AddSplash(LogoSplashPtr splash)
{
	EE_ASSERT(splash);
	m_splashScreens.push_back(splash);
}

void GameLoadingState::QueueBlockFile(const efd::AssetID& blockFile, efd::Bool critical)
{
	Loader block;
	block.m_blockFile = blockFile;
	block.m_critical = critical;
	block.m_queued = false;

	m_blockFiles.push_back(block);
}

efd::Bool GameLoadingState::LoadingIsDone() const
{
	return (m_blockFiles.empty() && m_splashScreens.empty() &&
		(!m_pCurrentSplash || m_pCurrentSplash->IsDone()));
}

// find the next available splash screen.
LogoSplashPtr GameLoadingState::GetNextSplash()
{
	// skip all remaining skippable ones if we are skipping.
// 	while (!m_splashScreens.empty() && m_splashScreens.front()->Abort())
// 	{
// 		m_splashScreens.pop_front();
// 	}

	if (!m_splashScreens.empty())
	{
		LogoSplashPtr pNext = m_splashScreens.front();
		m_splashScreens.pop_front();
		EE_ASSERT(pNext);

		EE_ASSERT(m_pRenderStep);
		pNext->OnInit(m_pMasterService->GetServiceManager());
		m_pRenderStep->AppendRenderClick(pNext);
		pNext->SetActive(true);
		return pNext;
	}
	return NULL;
}

// Ask the entity loader to load the given block file.
void GameLoadingState::LoadBlockFile(Loader& blockFile)
{
	// Ê¹ÓÃworldService¼ÓÔØ
// 	egf::EntityLoaderService* pELS =
// 		m_pMasterService->GetServiceManager()->GetSystemServiceAs<egf::EntityLoaderService>();
// 	egf::BlockLoadParameters blp(m_myCallbackCat);
// 	pELS->RequestEntitySetLoad(blockFile.m_blockFile, &blp);
// blockFile.m_queued = true;
 	GameWorldServicePtr pGameWorld = m_pMasterService->GetServiceManager()->GetSystemServiceAs<GameWorldService>();
 	EE_ASSERT(pGameWorld);
 	if (pGameWorld)
 	{
 		pGameWorld->registerObserver(this, Event::BlockLoad);
 		pGameWorld->LoadBlock(blockFile.m_blockFile.GetURN(), true);
 		blockFile.m_queued = true;
 	}
}


/// Do cleanup work
void GameLoadingState::Cleanup()
{
	m_splashScreens.clear();
	m_pCurrentSplash = NULL;
	m_pLastSplash = NULL;
	m_blockFiles.clear();	
	m_pRenderStep = NULL;
}

void GameLoadingState::HandleWorldLoadedMessage(
	const egf::EntityLoadResult* pWorldMessage,
	efd::Category targetChannel)
{
	EE_ASSERT_MESSAGE(pWorldMessage, ("LoadingStateEx recieved wrong message class"));

	if (!m_blockFiles.empty() && m_blockFiles.front().m_blockFile == pWorldMessage->m_assetID)
	{
		// we have files queued up for loading
		Loader& bf = m_blockFiles.front();
		// sanity check
		EE_ASSERT(bf.m_queued);

		// If we receive an EntityCreationCompleted message,
		// then the first queued block file must be ready.
		if (pWorldMessage->m_result == egf::EntityLoadResult::elr_EntityCreationCompleted)
		{
			EE_LOG(efd::kApp, efd::ILogger::kLVL2,
				("Asset '%s' was loaded successfully.", bf.m_blockFile.c_str()));
		}
		else if ((pWorldMessage->m_result == egf::EntityLoadResult::elr_Failed) ||
			(pWorldMessage->m_result == egf::EntityLoadResult::elr_RequestFailure) ||
			(pWorldMessage->m_result == egf::EntityLoadResult::elr_EntitySetNotFound) ||
			(pWorldMessage->m_result == egf::EntityLoadResult::elr_AssetIDNotFound))
		{
			efd::utf8string msg(efd::Formatted, "A %s file has failed to load (%s).",
				bf.m_critical ? "critical" : "non-critical",
				bf.m_blockFile.c_str());

			EE_LOG(efd::kApp, bf.m_critical ? efd::ILogger::kERR0 : efd::ILogger::kERR2,
				("%s", msg.c_str()));			
		}
		else
		{
			EE_ASSERT(pWorldMessage->Succeeded());
		}
		m_blockFiles.pop_front();
		if (!m_blockFiles.empty())
		{
			LoadBlockFile(m_blockFiles.front());
		}
	}
}

void GameLoadingState::OnEvent(Event::EventType event, Event::Subject* pSubject,void *arg)
{
	if(Event::BlockLoad == event)
	{
		const egf::EntityLoadResult* pWorldMessage = (const egf::EntityLoadResult*)arg;

		if (!m_blockFiles.empty() && m_blockFiles.front().m_blockFile == pWorldMessage->m_assetID)
		{
			if (pWorldMessage->m_result == egf::EntityLoadResult::elr_EntityCreationCompleted)
			{
				EE_LOG(efd::kApp, efd::ILogger::kLVL2,
					("Asset '%s' was loaded successfully.", pWorldMessage->m_assetID.GetURN().c_str()));
			}
			else if ((pWorldMessage->m_result == egf::EntityLoadResult::elr_Failed) ||
				(pWorldMessage->m_result == egf::EntityLoadResult::elr_RequestFailure) ||
				(pWorldMessage->m_result == egf::EntityLoadResult::elr_EntitySetNotFound) ||
				(pWorldMessage->m_result == egf::EntityLoadResult::elr_AssetIDNotFound))
			{
				EE_ASSERT(FALSE && "load block error" );
				return;
			}
			else
			{
				EE_ASSERT(pWorldMessage->Succeeded());
			}
			m_blockFiles.pop_front();
			if (!m_blockFiles.empty())
			{
				LoadBlockFile(m_blockFiles.front());
			}
		}		
	}
}