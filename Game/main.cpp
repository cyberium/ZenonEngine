#include "stdafx.h"

const wchar_t* c_RenderWindow_ClassNameW = L"RenderWindowClass";

void main_internal(int argumentCount, char* arguments[])
{
	// 1. Initialize engine and some improtant managers
	IBaseManager* BaseManager = InitializeEngine(ArgumentsToVector(argumentCount, arguments));

	// 2. Set file location
	GetManager<IFilesManager>(BaseManager)->RegisterFilesStorage(std::make_shared<CLocalFilesStorage>("D:\\_programming\\ZenonEngine\\gamedata\\"));

	// 3. Create application
	Application app(BaseManager, ::GetModuleHandle(NULL));

	CWindowClassRegistratorObject windowRegistrator(&app);
	windowRegistrator.RegisterWindowClass(c_RenderWindow_ClassNameW);

	CWindowObject firstWindowObject;
	firstWindowObject.CreateWindowInstance
	(
		&app,
		&windowRegistrator,
		L"First window name",
		GetManager<ISettings>(BaseManager)->GetGroup("Video")->GetSettingT<glm::vec2>("WindowSize")->Get().x,
		GetManager<ISettings>(BaseManager)->GetGroup("Video")->GetSettingT<glm::vec2>("WindowSize")->Get().y
	);

	std::shared_ptr<IRenderDevice> renderDevice = app.CreateRenderDevice(RenderDeviceType::RenderDeviceType_DirectX);
	AddManager<IRenderDevice>(BaseManager, renderDevice);

	std::shared_ptr<IRenderWindow> firstRenderWindow = renderDevice->CreateRenderWindow(&firstWindowObject, true);
	app.AddRenderWindow(firstRenderWindow);

	std::shared_ptr<IFontsManager> fontsManager = std::make_shared<FontsManager>(BaseManager);
	AddManager<IFontsManager>(BaseManager, fontsManager);

	std::shared_ptr<IGameState> gs1 = std::make_shared<CGameState_World>(&app, firstRenderWindow);
	app.SetGameState(gs1);

	app.Run();

}

int main(int argumentCount, char* arguments[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	int * ttt = new int[250];

	main_internal(argumentCount, arguments);		

	_CrtMemDumpAllObjectsSince(NULL);

	return 0;
}
