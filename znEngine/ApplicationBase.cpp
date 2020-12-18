#include "stdafx.h"

// Genreal
#include "ApplicationBase.h"

// Additional
#include "znEngine.h"

CApplicationBase::CApplicationBase()
	: m_IsInitialized(false)
	, m_IsRunning(false)
	, m_GameDeltaTime(0.0)
    , m_GameTime(0.0)
    , m_FrameCounter(0L)
{
}


CApplicationBase::~CApplicationBase()
{

}


int CApplicationBase::Run()
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

void CApplicationBase::Stop()
{
	m_PluginsManager->FinalizeAllPlugins();

	m_RenderDevice.reset();

	m_BaseManager.reset();

	m_PluginsManager->RemoveAllPlugins();

	ExitApplication();
}




//
// IApplication
//
IRenderDevice& CApplicationBase::CreateRenderDevice(RenderDeviceType DeviceType)
{
	m_RenderDevice = GetBaseManager().GetManager<IznRenderDeviceFactory>()->GetRenderDeviceCreator(DeviceType).CreateRenderDevice();
	return GetRenderDevice();
}

void CApplicationBase::AddRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow)
{
	const auto& it = std::find(m_Windows.begin(), m_Windows.end(), RenderWindow);
	if (it != m_Windows.end())
		throw CException("CApplicationBase: RenderWindow already exists.");
	m_Windows.push_back(RenderWindow);
}

void CApplicationBase::CloseRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow)
{
	const auto& it = std::find(m_Windows.begin(), m_Windows.end(), RenderWindow);
	if (it == m_Windows.end())
		throw CException("CApplicationBase: Unable find RenderWindow.");
	m_Windows.erase(it);
}

void CApplicationBase::DoBeforeRun()
{
	if (m_IsInitialized)
		return;

	for (const auto& w : m_Windows)
		if (auto applicationEventsListener = std::dynamic_pointer_cast<IApplicationEventsListener>(w))
			applicationEventsListener->OnInitialize(EventArgs());

	m_IsInitialized = true;
	m_IsRunning = true;
}

int CApplicationBase::DoRun()
{
	HighResolutionTimer gameTimer;

	int processResult = ProcessEvents();

	m_BaseManager->GetManager<ILog>()->PushAllMessages();

	UpdateEventArgs updateArgs(m_GameDeltaTime, m_GameTime, m_FrameCounter);
	for (const auto& w : m_Windows)
		if (auto applicationEventsListener = std::dynamic_pointer_cast<IApplicationEventsListener>(w))
			applicationEventsListener->OnUpdate(updateArgs);

	m_GameDeltaTime = gameTimer.GetElapsedMilliSeconds();
	m_GameTime += m_GameDeltaTime;
	m_FrameCounter++;

	return processResult;
}

void CApplicationBase::DoAfterRun()
{
	for (const auto& w : m_Windows)
		if (auto applicationEventsListener = std::dynamic_pointer_cast<IApplicationEventsListener>(w))
			applicationEventsListener->OnExit(EventArgs());
	m_Windows.clear();

	// Save settings
	GetBaseManager().GetManager<ISettings>()->Save();
}

double CApplicationBase::GetGameDeltaTime() const
{
	return m_GameDeltaTime;
}

double CApplicationBase::GetGameTime() const
{
	return m_GameTime;
}

size_t CApplicationBase::GetFrameCounter() const
{
	return m_FrameCounter;
}

IBaseManager& CApplicationBase::GetBaseManager() const
{
	return *(m_BaseManager.get());
}

IRenderDevice& CApplicationBase::GetRenderDevice() const
{
	_ASSERT(m_RenderDevice != nullptr);
	return *m_RenderDevice;
}



//
// Protected
//
void CApplicationBase::SetStopped()
{
	m_IsRunning = false;
}

