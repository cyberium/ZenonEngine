#include "stdafx.h"

// Additional
#include "EditorUIFrame.h"
#include <QApplication>

#include "BugTrap/BugTrap.h"

#include "Editor.h"
#include "Editor3DFrame.h"
#include "Editor3DPreviewScene.h"

static IBaseManager* BaseManager = nullptr;

void main_internal(int argc, char *argv[])
{
	// 1. Initialize engine and some improtant managers
	BaseManager = InitializeEngine(Utils::ArgumentsToVector(argc, argv), "");

	//try
	//{
		// 3. Create application
		Application app(*BaseManager, ::GetModuleHandle(NULL));
		CNativeWindowFactory nativeWindowFactory(&app);

		IRenderDevice& renderDevice = app.CreateRenderDevice(RenderDeviceType::RenderDeviceType_DirectX11);
		BaseManager->AddManager<IznFontsManager>(MakeShared(FontsManager, renderDevice, *BaseManager));

		BaseManager->GetManager<ILoader>()->Start();

		QApplication qtApplication(argc, argv);
		CEditor editor(*BaseManager);
		CEditorUIFrame editorUI(editor);
		
		// 'Editor3D' scene
		{
			auto nativeWindow = nativeWindowFactory.CreateWindowProxy(*editorUI.getMainEditor());
			auto renderWindow = renderDevice.GetObjectsFactory().CreateRenderWindow(std::move(nativeWindow), false);

			app.AddRenderWindow(renderWindow);

			std::shared_ptr<CEditor3DFrame> editorScene = MakeShared(CEditor3DFrame, editor, *renderWindow);
			editorScene->AddSceneEventsListener(&editorUI);
			editorScene->Initialize();

			renderWindow->SetRenderWindowEventListener(std::dynamic_pointer_cast<IRenderWindowEventListener>(editorScene));
			renderWindow->SetNativeWindowEventListener(std::dynamic_pointer_cast<IznNativeWindowEventListener>(editorScene));
		}

		// 'Preview' scene
		{
			auto nativeWindow = nativeWindowFactory.CreateWindowProxy(*editorUI.getModelPreview());
			auto renderWindow = renderDevice.GetObjectsFactory().CreateRenderWindow(std::move(nativeWindow), false);

			app.AddRenderWindow(renderWindow);

			std::shared_ptr<CEditor3DPreviewScene> sceneForPreview = MakeShared(CEditor3DPreviewScene, editor, *renderWindow);
			sceneForPreview->Initialize();

			renderWindow->SetRenderWindowEventListener(std::dynamic_pointer_cast<IRenderWindowEventListener>(sceneForPreview));
			renderWindow->SetNativeWindowEventListener(std::dynamic_pointer_cast<IznNativeWindowEventListener>(sceneForPreview));
		}

		editor.GetUIFrame().InitializeEditorFrame();
		editor.Get3DFrame().InitializeEditorFrame();
		editor.GetTools().Initialize();


		editorUI.showMaximized();


		std::shared_ptr<IDebugOutput> debugOutput(editorUI.getUI().LogViewer);
		BaseManager->GetManager<ILog>()->AddDebugOutput(debugOutput);
		
		QTimer timer(&editorUI);
		editorUI.connect(&timer, &QTimer::timeout, &editorUI, [&app] {
			app.DoRun();
		});

		app.DoBeforeRun();
		{
			timer.start();
			{
				QApplication::exec();
			}
			timer.stop();
		}
		app.DoAfterRun();

		BaseManager->GetManager<ILog>()->DeleteDebugOutput(debugOutput);

		QApplication::closeAllWindows();

	/*}
	catch (const CznRenderException& e)
	{
		Log::Fatal("RenderError: %s", e.MessageCStr());
	}
	catch (const CException& e)
	{
		Log::Fatal("EngienError: %s", e.MessageCStr());
	}*/
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

	if (BaseManager)
		delete BaseManager;

	//_CrtMemDumpAllObjectsSince(NULL);

	return 0;
}
