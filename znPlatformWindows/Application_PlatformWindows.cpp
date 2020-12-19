#include "stdafx.h"

// General
#include "Application_PlatformWindows.h"

// Additional
#include "DebugOutputConsole_PlatformWindows.h"

#include "NativeWindow_PlatformWindows.h"
#include "NativeWindowProxy_PlatformWindows.h"


namespace
{
	const wchar_t* cZenonWindowClassName = L"ZenonEngineWindowClass";

	LRESULT CALLBACK ZenonWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_CREATE)
		{

		}
		else if (message == WM_CLOSE)
		{
			Log::Warn("WndProc: WM_CLOSE.");

			// WM_QUIT
			PostQuitMessage(0);

			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		else if (message == WM_DESTROY)
		{
			Log::Warn("WndProc: WM_DESTROY.");

			return DefWindowProc(hwnd, message, wParam, lParam);
		}

		if (INativeWindow_WindowsSpecificEx* nativeWindow = (INativeWindow_WindowsSpecificEx*)::GetWindowLongPtrW(hwnd, GWLP_USERDATA))
			return nativeWindow->ProcessMessage(hwnd, message, wParam, lParam);

		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	typedef IznPlugin*(GetPluginFuncProc)(IBaseManager& BaseManager);
}




CApplication_PlatformWindows::CApplication_PlatformWindows()
	: CApplicationBase()
{
	m_HInstance = ::GetModuleHandle(NULL);

	HINSTANCE hDll;
	hDll = LoadLibrary(L"SHELL32.dll");

	WNDCLASSEX renderWindowClass = { 0 };
	renderWindowClass.cbSize = sizeof(WNDCLASSEX);
	renderWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	renderWindowClass.lpfnWndProc = &ZenonWndProc;
	renderWindowClass.cbClsExtra = 0;
	renderWindowClass.cbWndExtra = 0;
	renderWindowClass.hInstance = GetHInstance();
	renderWindowClass.hIcon = LoadIcon(hDll, MAKEINTRESOURCE(2));
	renderWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	renderWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	renderWindowClass.lpszMenuName = NULL;
	renderWindowClass.lpszClassName = cZenonWindowClassName;
	renderWindowClass.hIconSm = LoadIcon(hDll, MAKEINTRESOURCE(2));

	if (::RegisterClassExW(&renderWindowClass) == FALSE)
		throw CException("CNativeWindowFactory_PlatformWindows: Failed to register the render window class.");

	PreInitializeEngine();

	GetBaseManager().GetManager<ILog>()->AddDebugOutput(MakeShared(CDebugOutputConsole_PlatformWindows));

	InitializeEngineInternal();
}

CApplication_PlatformWindows::~CApplication_PlatformWindows()
{
	if (::UnregisterClassW(cZenonWindowClassName, GetHInstance()) == FALSE)
		Log::Error("CNativeWindowFactory_PlatformWindows: Failed to unregister render window class.");
}



//
// IApplication
//
std::unique_ptr<IznNativeWindow> CApplication_PlatformWindows::CreateNativeWindow(const std::string & WindowName, glm::ivec2 WindowSize) const
{
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	RECT windowRect = { 0, 0, WindowSize.x, WindowSize.y };
	if (FALSE == ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE))
		throw CException("CNativeWindowFactory_PlatformWindows: Failed to AdjustWindowRect.");

	LONG windowWidth = windowRect.right - windowRect.left;
	LONG windowHeight = windowRect.bottom - windowRect.top;

	int windowX = (screenWidth - windowWidth) / 2;
	int windowY = (screenHeight - windowHeight) / 2;

	HWND hWnd = CreateWindowExW
	(
		NULL,
		cZenonWindowClassName,
		Resources::utf8_to_utf16(WindowName).c_str(),
		WS_OVERLAPPEDWINDOW,
		windowX, windowY, windowWidth, windowHeight,
		NULL,
		NULL,
		GetHInstance(),
		NULL
	);

	if (hWnd == NULL)
		throw CException("CNativeWindowFactory_PlatformWindows: Failed to create render window.");

	if (::ShowWindow(hWnd, SW_SHOWDEFAULT) == FALSE)
	{
		//	throw CException("CNativeWindowFactory_PlatformWindows: Failed to show window.");
	}

	if (::BringWindowToTop(hWnd))
	{
		//	throw CException("CNativeWindowFactory_PlatformWindows: Failed to bring window to top.");
	}

	std::unique_ptr<IznNativeWindow> nativeWindow = std::make_unique<CNativeWindow_PlatformWindows>(hWnd);
	SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)(dynamic_cast<INativeWindow_WindowsSpecificEx*>(nativeWindow.get())));
	return std::move(nativeWindow);
}

std::unique_ptr<IznNativeWindow> CApplication_PlatformWindows::CreateNativeWindowProxy(IznNativeWindow & SourceNativeWindow) const
{
	std::unique_ptr<IznNativeWindow> nativeWindow = std::make_unique<CNativeWindowProxy_PlatformWindows>(SourceNativeWindow);
	//SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)(dynamic_cast<INativeWindow_WindowsSpecificEx*>(nativeWindow.get())));
	return std::move(nativeWindow);
}



//
// IApplication_PlatformBase
//
void CApplication_PlatformWindows::ShowMessageBox(const std::string & Title, const std::string & Message) const
{
	MessageBoxA(HWND_DESKTOP, Message.c_str(), Title.c_str(), MB_ICONERROR | MB_OK);
}

std::string CApplication_PlatformWindows::GetExecutablePath() const
{
	char buffer[MAX_PATH];
	if (FALSE == GetModuleFileNameA(NULL, buffer, MAX_PATH))
		throw CException("GetExecutablePath: Error.");
	std::string f(buffer);
	return f.substr(0, f.find_last_of("\\/"));
}

IznPlugin* CApplication_PlatformWindows::LoadPlugin(const std::string & PluginFilename) const
{
	try
	{
		HMODULE hModule = ::LoadLibraryA(PluginFilename.c_str());
		if (hModule == NULL)
			throw CPluginException_NotAPlguin("Error while ::LoadLibrary().");

		void* getPluginProcNative = ::GetProcAddress(hModule, "GetPlugin");
		if (getPluginProcNative == NULL)
		{
			::FreeLibrary(hModule);
			throw CPluginException_NotAPlguin("Function 'GetPlugin' not found.");
		}

		IznPlugin* plugin = ((GetPluginFuncProc*)getPluginProcNative)(GetBaseManager());
		if (plugin == nullptr)
		{
			::FreeLibrary(hModule);
			throw CPluginException("Error while create plugin object.");
		}

		Log::Green("Plugin '%s' loaded.", PluginFilename.c_str());
		return plugin;
	}
	catch (const CPluginException_NotAPlguin& e)
	{
		Log::Warn("Plugin '%s' is not a ZenonEngine plugin.", PluginFilename.c_str());
		Log::Warn("--->%s", e.MessageCStr());
		return nullptr;
	}
	catch (const CPluginException& e)
	{
		throw CException("Plugin '%s' loaded with error. '%s'.", PluginFilename.c_str(), e.MessageCStr());
	}

	throw CException("Unexpected code");
}

void CApplication_PlatformWindows::UnloadPlugin(const std::string& PluginFilename) const
{
	HMODULE hModule = ::GetModuleHandleA(PluginFilename.c_str());
	if (hModule == NULL)
		throw CPluginException("Failed to '::GetModuleHandleA()'.");

	if (FALSE == ::FreeLibrary(hModule))
		throw CPluginException("Failed to '::FreeLibrary()'.");
}



//
// IApplication_WindowsSpecific
//
HINSTANCE CApplication_PlatformWindows::GetHInstance() const
{
	return m_HInstance;
}



//
// Protected
//
int CApplication_PlatformWindows::ProcessEvents()
{
	MSG msg = { 0 };
	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			SetStopped();
			return static_cast<int>(msg.wParam);
		}

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return 1;
}

void CApplication_PlatformWindows::ExitApplication()
{
	PostQuitMessage(0);
}
