// ======================================================================================
// File         : main.cpp
// Author       : Wu Jie 
// Last Change  : 10/12/2010 | 11:44:16 AM | Tuesday,October
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////

#include "game_pch.h"

#include <NiStandardAllocator.h>

#include <efd/FileDestination.h>
#include <efd/ILogger.h>
#include <efd/MemoryDefines.h>
#include <NiStaticDataManager.h>

#include "GameApp.h"

#define TITLE_SERVICE_NAME GameApp

//--------------------------------------------------------------------------------------------------
#if defined(EE_PLATFORM_WIN32)
// Support for D3D renderers

// DX9 renderer
#include <NiDX9Renderer.h>
#include <NiDX9RendererSetup.h>

// D3D10 renderer
#include <NiD3D10Renderer.h>
#include <NiD3D10RendererSetup.h>

// D3D11 renderer
#include <ecrD3D11Renderer/D3D11Renderer.h>
#include <ecrD3D11RendererSetup/D3D11RendererSetup.h>

#endif

// Make use of the default memory allocator implementation.  For instructions
// on how to use a custom allocator, see the notes in <efd/MemoryDefines.h>.
#include <efd/DefaultInitializeMemoryManager.h>
EE_USE_DEFAULT_ALLOCATOR;

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------
int GamebryoMain(
    efd::InstanceRef instance,
    efd::InstanceRef previousInstance,
    CmdLineType commandLine,
    int argcInit,
    char** argvInit)
{
    // Initialize Gamebryo
    NiInitOptions* pInitOptions = EE_EXTERNAL_NEW NiInitOptions();
    NiInit(pInitOptions);

    // Disable writing the memory log to disk (saves time when it is not needed)
    if (efd::MemTracker::Get())
        efd::MemTracker::Get()->SetWriteToLog(false);

    // Run the title
    efd::SmartPointer<GameApp> spApp = EE_NEW GameApp();
    int returnCode = spApp->Go(instance, previousInstance, commandLine, argcInit, argvInit);
    spApp = NULL;

    // Shut down Gamebryo
    NiShutdown();
    EE_EXTERNAL_DELETE pInitOptions;

    return returnCode;
}
//--------------------------------------------------------------------------------------------------
#if defined(EE_PLATFORM_WIN32)
int WINAPI WinMain(HINSTANCE hI, HINSTANCE hPI, CmdLineType cmdline, int /*iWinMode*/)
{
    return GamebryoMain(hI, hPI, cmdline, __argc, __argv);
}
#else
int main(int argc, char* argv[])
{
    return GamebryoMain(NULL, NULL, NULL, argc, argv);
}
#endif
//--------------------------------------------------------------------------------------------------
