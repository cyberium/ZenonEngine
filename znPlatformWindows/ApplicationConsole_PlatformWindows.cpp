#include "stdafx.h"

// General
#include "ApplicationConsole_PlatformWindows.h"

// Additional
#include "DebugOutputConsole_PlatformWindows.h"

#include "NativeWindow_PlatformWindows.h"
#include "NativeWindowProxy_PlatformWindows.h"


namespace
{
	typedef IznPlugin*(GetPluginFuncProc)(IBaseManager& BaseManager);
}




CApplicationConsole_PlatformWindows::CApplicationConsole_PlatformWindows()
{
	m_HInstance = ::GetModuleHandle(NULL);

	PreInitializeEngine();

	GetBaseManager().GetManager<ILog>()->AddDebugOutput(MakeShared(CDebugOutputConsole_PlatformWindows));

	InitializeEngineInternal({});
}

CApplicationConsole_PlatformWindows::~CApplicationConsole_PlatformWindows()
{

}



//
// IApplication_PlatformBase
//
void CApplicationConsole_PlatformWindows::ShowMessageBox(const std::string & Title, const std::string & Message) const
{
	MessageBoxA(HWND_DESKTOP, Message.c_str(), Title.c_str(), MB_ICONERROR | MB_OK);
}

std::string CApplicationConsole_PlatformWindows::GetExecutablePath() const
{
	char buffer[MAX_PATH];
	if (FALSE == GetModuleFileNameA(NULL, buffer, MAX_PATH))
		throw CException("GetExecutablePath: Error.");
	std::string f(buffer);
	return f.substr(0, f.find_last_of("\\/"));
}

IznPlugin* CApplicationConsole_PlatformWindows::LoadPlugin(const std::string & PluginFilename) const
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

void CApplicationConsole_PlatformWindows::UnloadPlugin(const std::string& PluginFilename) const
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
HINSTANCE CApplicationConsole_PlatformWindows::GetHInstance() const
{
	return m_HInstance;
}



//
// Protected
//
int CApplicationConsole_PlatformWindows::ProcessEvents()
{
	return 1;
}

void CApplicationConsole_PlatformWindows::ExitApplication()
{
	PostQuitMessage(0);
}
