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
	IBaseManager* BaseManager = InitializeEngine(ArgumentsToVector(argc, argv), "");

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

	std::shared_ptr<IScene> scene = BaseManager->GetManager<IScenesFactory>()->CreateScene("SceneDefault");
	scene->ConnectEvents(dynamic_cast<IRenderWindowEvents*>(renderWindow));
	scene->Initialize();

	w.ApplyScene(scene);
	w.ApplyTest();

	w.show();

	BaseManager->GetManager<ILog>()->AddDebugOutput(std::make_shared<DebugOutput_EditorLog>(w.getUI().LogTextEdit));

	app.Run();

	return a.exec();
}
