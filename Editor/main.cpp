#include "stdafx.h"

// Additional
#include "MainEditor.h"
#include <QtWidgets/QApplication>

#include "DebugOutputEditorLog.h"
#include "RenderWindowWidget.h"

#include "GameState_Editor.h"

void RenderThread(Application* app)
{
	app->Run();
}

int main(int argc, char *argv[])
{
	std::string Test = "Hello";
	// 1. Initialize engine and some improtant managers
	IBaseManager* BaseManager = InitializeEngine(ArgumentsToVector(argc, argv), Test);

	// 3. Create application
	Application app(BaseManager, ::GetModuleHandle(NULL));

	QApplication a(argc, argv);
	MainEditor w;

	IRenderDevice* renderDevice = app.CreateRenderDevice(RenderDeviceType::RenderDeviceType_DirectX);
	BaseManager->AddManager<IRenderDevice>(std::shared_ptr<IRenderDevice>(renderDevice));

	std::shared_ptr<IFontsManager> fontsManager = std::make_shared<FontsManager>(renderDevice, BaseManager);
	BaseManager->AddManager<IFontsManager>(fontsManager);

	// Render window for main editor
	IRenderWindow* renderWindow = renderDevice->CreateRenderWindow(w.getUI().EditorWindow, false);
	app.AddRenderWindow(renderWindow);

	std::shared_ptr<IGameState> gameState = BaseManager->GetManager<IGameStatesFactory>()->CreateGameStateWithHighestPriority(renderWindow, w.getUI().EditorWindow);
	//std::shared_ptr<CGameState_Editor> gameState = std::make_shared<CGameState_Editor>(BaseManager, renderWindow, w.getUI().EditorWindow, &w);
	app.SetGameState(gameState);

	w.ApplyScene(std::dynamic_pointer_cast<IScene>(app.GetGameState()->GetScene()));
	w.ApplyTest();

	w.show();

	BaseManager->GetManager<ILog>()->AddDebugOutput(std::make_shared<DebugOutput_EditorLog>(w.getUI().LogTextEdit));

	std::thread renderThread(RenderThread, &app);
	renderThread.detach();

	return a.exec();
}
