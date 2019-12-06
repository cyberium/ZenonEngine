#include "stdafx.h"

// Genreal
#include "Application.h"

//---------------------------------------------------------------
//-- EVENTS
//---------------------------------------------------------------

Event & Application::Initialize()
{
	return m_Initialize;
}

bool Application::OnInitialize(EventArgs& e)
{
	if (m_bIsInitialized)
		return true;

	bool result = m_Initialize(e);

	m_bIsInitialized = true;

	return result;
}

UpdateEvent& Application::Update()
{
	return m_Update;
}

bool Application::OnUpdate(UpdateEventArgs& e)
{
	return m_Update(e);
}

Event & Application::Terminate()
{
	return m_Terminate;
}

bool Application::OnTerminate(EventArgs& e)
{
	return m_Terminate(e);
}

Event & Application::Terminated()
{
	return m_Terminated;
}

bool Application::OnTerminated(EventArgs& e)
{
	m_bIsInitialized = false;

	return m_Terminated(e);
}

Event & Application::Exit()
{
	return m_Exit;
}

bool Application::OnExit(EventArgs& e)
{
	bool result = m_Exit(e);

	// Destroy any windows that are still hanging around.
	for (const auto& it : m_Windows)
		if (it.second != nullptr)
			it.second->CloseWindow();

	// Setting this to false will cause the main application's message pump to stop.
	m_bIsRunning = false;

	return result;
}

UserEvent & Application::UserEvent()
{
	return m_UserEvent;
}

bool Application::OnUserEvent(UserEventArgs& e)
{
	return m_UserEvent(e);
}


LRESULT Application::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const auto& window = m_Windows.find(hwnd);
	if (window != m_Windows.end())
	{
		return window->second->WndProc(hwnd, message, wParam, lParam);
	}

	if (message == WM_CREATE)
		return 0;

	return DefWindowProc(hwnd, message, wParam, lParam);
}
