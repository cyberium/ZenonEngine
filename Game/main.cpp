#include "stdafx.h"

void main_internal(int argumentCount, char* arguments[])
{
	// 1. Initialize engine and some improtant managers
	IBaseManager* BaseManager = InitializeEngine(ArgumentsToVector(argumentCount, arguments));

	// 3. Create application
	Application app(BaseManager, ::GetModuleHandle(NULL));

	CWindowObject firstWindowObject;
	firstWindowObject.CreateWindowInstance
	(
		&app,
		L"First window name",
		GetManager<ISettings>(BaseManager)->GetGroup("Video")->GetSettingT<glm::vec2>("WindowSize")->Get().x,
		GetManager<ISettings>(BaseManager)->GetGroup("Video")->GetSettingT<glm::vec2>("WindowSize")->Get().y
	);

	std::shared_ptr<IRenderDevice> renderDevice = app.CreateRenderDevice(RenderDeviceType::RenderDeviceType_DirectX);
	AddManager<IRenderDevice>(BaseManager, renderDevice);

	std::shared_ptr<IFontsManager> fontsManager = std::make_shared<FontsManager>(renderDevice, BaseManager);
	AddManager<IFontsManager>(BaseManager, fontsManager);

	std::shared_ptr<IRenderWindow> firstRenderWindow = renderDevice->CreateRenderWindow(&firstWindowObject, true);
	app.AddRenderWindow(firstRenderWindow);

	std::shared_ptr<IGameState> gameState = GetManager<IGameStatesFactory>(BaseManager)->CreateGameStateWithHighestPriority(firstRenderWindow, &firstWindowObject);
	app.SetGameState(gameState);

	app.Run();

}

int main(int argumentCount, char* arguments[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	main_internal(argumentCount, arguments);		

	_CrtMemDumpAllObjectsSince(NULL);

	return 0;
}
