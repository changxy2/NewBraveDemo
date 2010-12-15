#pragma once
#include "GameState.h"
#include "LogoSplash.h"

#include <NiScreenFillingRenderView.h>
#include <NiDefaultClickRenderStep.h>
#include <egf/EntityLoaderService.h>

#include "Observer.h"

class GameLoadingState : 	public GameState , public Event::Observer
{
	EE_DECLARE_CLASS1(GameLoadingState, game::CLASS_ID_GameLoadingState, GameState);
	EE_DECLARE_CONCRETE_REFCOUNT;

public:
	GameLoadingState(GameStateService* pMasterService, GameStateID nextID);
	virtual ~GameLoadingState(void);

	virtual void OnInit();
	virtual void OnTick(efd::TimeType time);
	virtual void OnShutdown();

	virtual efd::Bool Enter();
	virtual efd::Bool Leave();

	void AddSplash(LogoSplashPtr splash);

	void QueueBlockFile(const efd::AssetID& blockFile, efd::Bool critical);

	void HandleWorldLoadedMessage(const egf::EntityLoadResult* pWorldMessage,efd::Category targetChannel);

	void OnEvent(Event::EventType event, Event::Subject* pSubject,void *arg);

protected:
	struct Loader
	{
		efd::AssetID m_blockFile;
		/// If true and the file fails to load then the whole load process fails.
		efd::Bool m_critical;
		/// Set true when the file has been sent to the entity loader.
		efd::Bool m_queued;
	};

	typedef efd::list<LogoSplashPtr> SplashScreens;
	typedef efd::list<Loader> BlockFiles;

	NiDefaultClickRenderStepPtr m_pRenderStep;
	LogoSplashPtr m_pCurrentSplash;         ///< Current splash screens
	LogoSplashPtr m_pLastSplash;            ///< Last splash screen to be displayed,
	///< held until current one is ready.
	SplashScreens m_splashScreens;          ///< all splash screens that are not current.	
	BlockFiles m_blockFiles;                ///< all block files to be loaded.

	efd::Category m_myCallbackCat;

	virtual efd::Bool LoadingIsDone() const;

	// find the next available splash screen.
	LogoSplashPtr GetNextSplash();

	// Ask the entity loader to load the given block file.
	void LoadBlockFile(Loader& blockFile);

	/// Initialize ourselves
	void Init();

	/// Do cleanup work
	void Cleanup();
};
