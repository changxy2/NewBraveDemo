// ======================================================================================
// File         : GameApp.h
// Author       : Wu Jie 
// Last Change  : 10/12/2010 | 11:43:42 AM | Tuesday,October
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef GAMEAPP_H_1286855024
#define GAMEAPP_H_1286855024
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////
//加入 PhysXSDKManager 用来操作Physx 对象
#include <efdPhysX/PhysXSDKManager.h>    
//PhysX Service 的创建头文件
#include <egfPhysX/ServiceAllocator.h>    

#include <efd/ServiceManager.h>
#include <egf/EntityChangeMessage.h>
#include <ecrInput/InputActionMessage.h>

#if defined(EE_PLATFORM_WIN32)
typedef LPTSTR CmdLineType;
#else
typedef const char* CmdLineType;
#endif


///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

/**
   The GameApp class is a system service that represents the game itself. It is responsible
   for setting up all the other system services required by the game, starting up the
   ServiceManager which in turn runs the game's main loop and loading the main block file.
   It also creates the default camera. GameApp is built using a basic set of system
   services.
*/

//------------------------------------------------------------------------------------------------
class GameApp : public efd::ISystemService
{
    EE_DECLARE_CLASS1 (GameApp, game::SERVICE_ID_GameApp, IBase);
    EE_DECLARE_CONCRETE_REFCOUNT;

public:
    /**
        Constructor - Perform standard Gamebryo initialization and create a ServiceManager.
    */
    GameApp();

    /**
        Destructor - Perform standard Gamebryo cleanup and deletes the ServiceManager.
    */
    ~GameApp();

    /**
        Called from main to start the game. It sets up the required system services and
        starts up ServiceManager.

        @param instance Standard WinMain parameter. Not used by other platforms. It
            is a handle to the current instance of the application.
        @param previous Standard WinMain parameter. Not used by other platforms.
        @param commandLine Standard WinMain parameter. Not used by other platforms.
            It is a pointer to the command line for the application.
        @param argcInit Number of command line parameters.
        @param argvInit Command line arguments.
        @return 0 if the game runs OK and 1 if game fails to start correctly.
    */
    efd::SInt32 Go(
        efd::InstanceRef instance,
        efd::InstanceRef previous,
        CmdLineType commandLine,
        int argcInit,
        char** argvInit);

    /**
        A message handler function for the EntityChangeMessage message. This message
        is sent by the EntityManager in response to an entity create request. We
        use this to know when our camera has been created and associate it with the
        render surface.

        @param pMessage EntityChangeMessage message.
        @param targetCategory Category the message was sent on.
     */
    void HandleCameraDiscoverMessage(
        const egf::EntityChangeMessage* pMessage,
        efd::Category targetCategory);

    /**
    A message handler function for the InputActionMessage message. That message
    is sent by the InputService when it detects user input. This method looks for
    an event that indicates that the user wants to end the game. If that event is
    seen then it shuts down the ServiceManager which in turn ends the game.

    @param pMessage The InputActionMessage message.
    @param targetChannel Category the message was sent on.
    */
    void HandleActionMessage(const ecrInput::InputActionMessage* pMessage,
        efd::Category targetChannel);

    virtual const char* GetDisplayName() const;

	void SetupGameStates();

protected:
	// We register dependencies with other services during preInit
	virtual efd::SyncResult GameApp::OnPreInit(efd::IDependencyRegistrar* pDependencyRegistrar);

    // We create the renderer and setup some input actions during init.
    virtual efd::AsyncResult OnInit();

    // The OnTick handler allows us to perform any periodic processing.
    virtual efd::AsyncResult OnTick();

    // Release resources.
    virtual efd::AsyncResult OnShutdown();

    void InitLoggingDestinations();
    bool SetupServices(
        efd::InstanceRef instance,
        efd::InstanceRef previous,
        CmdLineType commandLine,
        int argcInit,
        char** argvInit);
    void SetupInputActions();

private:

	
    // As the "Application" object I hold the primary reference count for the service manager.
    // Since I'm also an ISystemService myself this means I also have a plain pointer to the
    // service manager, but that pointer is automatically managed by the ServiceManager itself
    // so I need a separate pointer for application usage.
    efd::ServiceManagerPtr m_spServiceManager;

    // Category we use for additional action messages
    efd::Category m_catQuitGame;

	//PhysXSDK 实例
	efdPhysX::PhysXSDKManager* m_pPhysXSDKManager;


	//PhysX 内存分配器
	efdPhysX::PhysXAllocator* m_pPhysXAllocator;

};

// #################################################################################
#endif // END GAMEAPP_H_1286855024
// #################################################################################
