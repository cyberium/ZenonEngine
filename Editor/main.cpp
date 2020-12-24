#include "stdafx.h"

// Additional
#include "EditorUI/EditorUIFrame.h"
#include <QApplication>

#include "BugTrap/BugTrap.h"

#include "Editor.h"
#include "Editor3D/Editor3DFrame.h"
#include "Editor3D/Editor3DPreviewScene.h"

void main_internal(int argc, char *argv[])
{
	CApplication_PlatformWindows app;
	//Application app(Utils::ArgumentsToVector(argc, argv), ::GetModuleHandle(NULL));

	app.GetBaseManager().GetManager<IFilesManager>()->AddStorage(EFilesStorageType::GAMEDATA, MakeShared(CLocalFilesStorage, ""));

	IRenderDevice& renderDevice = app.CreateRenderDevice(RenderDeviceType::RenderDeviceType_DirectX11);
	app.GetBaseManager().AddManager<IznFontsManager>(MakeShared(FontsManager, renderDevice, app.GetBaseManager()));

	QApplication qtApplication(argc, argv);
	CEditor editor(app.GetBaseManager());
	CEditorUIFrame editorUI(editor);
		

	// 'Editor3D' scene
	{
		auto nativeWindow = app.CreateNativeWindowProxy(*editorUI.getMainEditor());
		auto renderWindow = renderDevice.GetObjectsFactory().CreateRenderWindow(std::move(nativeWindow), true);

		app.AddRenderWindow(renderWindow);

		std::shared_ptr<CEditor3DFrame> editorScene = MakeShared(CEditor3DFrame, editor, *renderWindow);
		editorScene->AddSceneEventsListener(&editorUI);
		editorScene->Initialize();

		renderWindow->SetRenderWindowEventListener(std::dynamic_pointer_cast<IRenderWindowEventListener>(editorScene));
		renderWindow->SetNativeWindowEventListener(std::dynamic_pointer_cast<IznNativeWindowEventListener>(editorScene));
	}

	// 'Preview' scene
	{
		auto nativeWindow = app.CreateNativeWindowProxy(*editorUI.getResourcePreview());
		auto renderWindow = renderDevice.GetObjectsFactory().CreateRenderWindow(std::move(nativeWindow), true);

		app.AddRenderWindow(renderWindow);

		std::shared_ptr<CEditor3DPreviewScene> sceneForPreview = MakeShared(CEditor3DPreviewScene, editor, *renderWindow);
		sceneForPreview->Initialize();

		renderWindow->SetRenderWindowEventListener(std::dynamic_pointer_cast<IRenderWindowEventListener>(sceneForPreview));
		renderWindow->SetNativeWindowEventListener(std::dynamic_pointer_cast<IznNativeWindowEventListener>(sceneForPreview));
	}

	editor.GetUIFrame().InitializeEditorFrame();
	editor.Get3DFrame().InitializeEditorFrame();
	editor.GetTools().Initialize();

	app.GetBaseManager().GetManager<IMaterialsFactory>()->PrintMaterials();

	editorUI.showMaximized();


	std::shared_ptr<IDebugOutput> debugOutput(editorUI.getUI().LogViewer);
	app.GetBaseManager().GetManager<ILog>()->AddDebugOutput(debugOutput);
		
	QTimer timer(&editorUI);
	editorUI.connect(&timer, &QTimer::timeout, &editorUI, [&app] {
		app.DoRun();
	});

	app.GetBaseManager().GetManager<ILoader>()->Start();

	app.DoBeforeRun();
	{
		timer.start();
		{
			QApplication::exec();
		}
		timer.stop();
	}
	app.DoAfterRun();

	app.GetBaseManager().GetManager<ILog>()->DeleteDebugOutput(debugOutput);

	QApplication::closeAllWindows();
}


int main(int argumentCount, char* arguments[])
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

	BT_InstallSehFilter(); 
	BT_SetAppName(L"ZenonEngine Editor"); 
	//BT_SetSupportEMail(L"your@email.com");
	BT_SetFlags(BTF_DETAILEDMODE /*| BTF_EDITMAIL*/); 
	//BT_SetSupportServer(L"localhost", 9999);
	//BT_SetSupportURL(L"http://www.your-web-site.com");

	main_internal(argumentCount, arguments);		

	return 0;
}
