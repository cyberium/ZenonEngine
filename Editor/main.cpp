#include "stdafx.h"

// Additional
#include "MainEditor.h"
#include <QApplication>

#include "DebugOutputEditorLog.h"
#include "RenderWindowWidget.h"

#include "Editor3DFrame.h"

static IBaseManager* BaseManager = nullptr;

void main_internal(int argc, char *argv[])
{
	// 1. Initialize engine and some improtant managers
	BaseManager = InitializeEngine(Utils::ArgumentsToVector(argc, argv), "");

	// 3. Create application
	Application app(*BaseManager, ::GetModuleHandle(NULL));

	QApplication a(argc, argv);
	MainEditor w;

	IRenderDevice& renderDevice = app.CreateRenderDevice(RenderDeviceType::RenderDeviceType_DirectX);

	std::shared_ptr<IFontsManager> fontsManager = std::make_shared<FontsManager>(renderDevice, *BaseManager);
	BaseManager->AddManager<IFontsManager>(fontsManager);

	// Render window for main editor
	std::shared_ptr<IRenderWindow> renderWindow = renderDevice.GetObjectsFactory().CreateRenderWindow(*w.getUI().EditorWindow, false);
	app.AddRenderWindow(renderWindow);

	BaseManager->GetManager<ILoader>()->Start();

	//std::shared_ptr<IScene> scene = BaseManager->GetManager<IScenesFactory>()->CreateScene("SceneDefault");
	std::shared_ptr<IScene> scene = std::make_shared<CSceneEditor>(*BaseManager);
	scene->ConnectEvents(std::dynamic_pointer_cast<IRenderWindowEvents>(renderWindow));
	scene->Initialize();

	w.ApplyScene(scene);
	w.ApplyTest();

	w.show();

	BaseManager->GetManager<ILog>()->AddDebugOutput(std::make_shared<DebugOutput_EditorLog>(w.getUI().LogTextEdit));

	app.Run();

	a.closeAllWindows();
}


int main(int argumentCount, char* arguments[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	_CrtSetBreakAlloc(158);

	main_internal(argumentCount, arguments);		

	delete BaseManager;

	_CrtMemDumpAllObjectsSince(NULL);

	return 0;
}
