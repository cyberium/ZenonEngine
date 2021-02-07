#include "stdafx.h"

// Genreal
#include "Application.h"

// Additional
#include "znEngine.h"

CApplication::CApplication()
	: m_IsInitialized(false)
	, m_IsRunning(false)
	, m_GameDeltaTime(0.0)
    , m_GameTime(0.0)
    , m_FrameCounter(0L)
{
}


CApplication::~CApplication()
{

}


int CApplication::Run()
{
	DoBeforeRun();

	int runResult = -1;
	while (m_IsRunning)
	{
		runResult = DoRun();
	}

	DoAfterRun();

	return runResult;
}

void CApplication::Stop()
{
	m_PluginsManager->FinalizeAllPlugins();

	OnStopAfterPluginManager();

	m_BaseManager.reset();

	m_PluginsManager->RemoveAllPlugins();

	ExitApplication();
}




//
// IApplication
//
void CApplication::DoBeforeRun()
{
	if (m_IsInitialized)
		return;

	DoBeforeRunImpl();

	m_IsInitialized = true;
	m_IsRunning = true;
}

int CApplication::DoRun()
{
	HighResolutionTimer gameTimer;

	int processResult = ProcessEvents();

	m_BaseManager->GetManager<ILog>()->PushAllMessages();

	UpdateEventArgs updateArgs(m_GameDeltaTime, m_GameTime, m_FrameCounter);
	DoRunImpl(updateArgs);

	m_GameDeltaTime = gameTimer.GetElapsedMilliSeconds();
	m_GameTime += m_GameDeltaTime;
	m_FrameCounter++;

	if (m_GameDeltaTime < 1)
		_sleep(1);

	return processResult;
}

void CApplication::DoAfterRun()
{
	DoAfterRunImpl();


	// Save settings
	GetBaseManager().GetManager<ISettings>()->Save();
}

double CApplication::GetGameDeltaTime() const
{
	return m_GameDeltaTime;
}

double CApplication::GetGameTime() const
{
	return m_GameTime;
}

size_t CApplication::GetFrameCounter() const
{
	return m_FrameCounter;
}

IBaseManager& CApplication::GetBaseManager() const
{
	return *(m_BaseManager.get());
}

IRenderDevice& CApplication::GetRenderDevice() const
{
	IRenderDevice * device = NULL;
	return *device;
}



//
// Protected
//
void CApplication::SetStopped()
{
	m_IsRunning = false;
}

