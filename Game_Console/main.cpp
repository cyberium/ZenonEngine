#include "stdafx.h"

#include "BugTrap/BugTrap.h"

void main_internal(int argumentCount, char* arguments[])
{
	CApplicationConsole_PlatformWindows app;

	{
		app.GetBaseManager().GetManager<ILoader>()->Start();
	}

	app.Run();

	app.Stop();
}


int main(int argumentCount, char* arguments[])
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

	BT_InstallSehFilter();
	BT_SetAppName(L"ZenonEngineConsole");
	//BT_SetSupportEMail(L"your@email.com");
	BT_SetFlags(BTF_DETAILEDMODE /*| BTF_EDITMAIL*/);
	//BT_SetSupportServer(L"localhost", 9999);
	//BT_SetSupportURL(L"http://www.your-web-site.com");

	main_internal(argumentCount, arguments);		

	return 0;
}
