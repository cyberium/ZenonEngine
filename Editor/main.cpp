#include "stdafx.h"

// Additional
#include "MainEditor.h"
#include <QtWidgets/QApplication>

#include "DebugOutputEditorLog.h"
#include "RenderWindowWidget.h"

#include "GameState_Editor.h"

int main(int argc, char *argv[])
{
	// 1. Initialize engine and some improtant managers
	IBaseManager* BaseManager = InitializeEngine(ArgumentsToVector(argc, argv));

	// 3. Create application
	Application app(BaseManager, ::GetModuleHandle(NULL));

	QApplication a(argc, argv);
	MainEditor w;

	std::shared_ptr<IRenderDevice> renderDevice = app.CreateRenderDevice(RenderDeviceType::RenderDeviceType_DirectX);
	AddManager<IRenderDevice>(BaseManager, renderDevice);

	std::shared_ptr<IFontsManager> fontsManager = std::make_shared<FontsManager>(renderDevice, BaseManager);
	AddManager<IFontsManager>(BaseManager, fontsManager);

	// Render window for main editor
	std::shared_ptr<IRenderWindow> renderWindow = renderDevice->CreateRenderWindow(w.getUI().EditorWindow, false);
	app.AddRenderWindow(renderWindow);

	//std::shared_ptr<IGameState> gameState = GetManager<IGameStatesFactory>(BaseManager)->CreateGameStateWithHighestPriority(renderWindow);
	std::shared_ptr<CGameState_Editor> gameState = std::make_shared<CGameState_Editor>(BaseManager, renderWindow, w.getUI().EditorWindow, &w);
	app.SetGameState(gameState);

	w.ApplyScene(std::dynamic_pointer_cast<IScene>(app.GetGameState()->GetScene3D()));
	w.ApplyTest();

	w.show();

	GetManager<ILog>(BaseManager)->AddDebugOutput(std::make_shared<DebugOutput_EditorLog>(w.getUI().LogTextEdit));

	app.Run();

	return a.exec();
}
