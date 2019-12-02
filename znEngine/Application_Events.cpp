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

void Application::OnInitialize(EventArgs& e)
{
	if (m_bIsInitialized)
		return;

	m_Initialize(e);

	m_bIsInitialized = true;
}

UpdateEvent& Application::Update()
{
	return m_Update;
}

void Application::OnUpdate(UpdateEventArgs& e)
{
	m_Update(e);
}

Event & Application::Terminate()
{
	return m_Terminate;
}

void Application::OnTerminate(EventArgs& e)
{
	m_Terminate(e);
}

Event & Application::Terminated()
{
	return m_Terminated;
}

void Application::OnTerminated(EventArgs& e)
{
	m_bIsInitialized = false;

	m_Terminated(e);
}

Event & Application::Exit()
{
	return m_Exit;
}

void Application::OnExit(EventArgs& e)
{
	m_Exit(e);

	// Destroy any windows that are still hanging around.
	for (const auto& it : m_Windows)
		if (it.second != nullptr)
			it.second->CloseWindow();

	// Setting this to false will cause the main application's message pump to stop.
	m_bIsRunning = false;
}

UserEvent & Application::UserEvent()
{
	return m_UserEvent;
}

void Application::OnUserEvent(UserEventArgs& e)
{
	m_UserEvent(e);
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