#include "stdafx.h"

// Additional
#include "EditorUIFrame.h"
#include <QApplication>

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

		CEditor editor(*BaseManager);

		IRenderDevice& renderDevice = app.CreateRenderDevice(RenderDeviceType::RenderDeviceType_DirectX11);

		std::shared_ptr<IznFontsManager> fontsManager = MakeShared(FontsManager, renderDevice, *BaseManager);
		BaseManager->AddManager<IznFontsManager>(fontsManager);
		BaseManager->GetManager<ILoader>()->Start();

		QApplication qtApplication(argc, argv);

		CEditorUIFrame editorUI(editor);
		editorUI.InitializeEditorFrame();

		{
			std::shared_ptr<IRenderWindow> renderWindow;
			std::shared_ptr<CEditor3DFrame> editorScene;
			{
				renderWindow = renderDevice.GetObjectsFactory().CreateRenderWindow(*editorUI.getMainEditor(), false);
				app.AddRenderWindow(renderWindow);

				editorScene = MakeShared(CEditor3DFrame, editor, *renderWindow);
				renderWindow->SetRenderWindowEventListener(std::dynamic_pointer_cast<IRenderWindowEventListener>(editorScene));
				renderWindow->SetNativeWindowEventListener(std::dynamic_pointer_cast<INativeWindowEventListener>(editorScene));
				editorScene->Initialize();
			}

			editorScene->InitializeEditorFrame();

			// Scene for preview
			{
				std::shared_ptr<IRenderWindow> renderWindowForModelPreview = renderDevice.GetObjectsFactory().CreateRenderWindow(*editorUI.getModelPreview(), false);
				app.AddRenderWindow(renderWindowForModelPreview);

				std::shared_ptr<CEditor3DPreviewScene> sceneForPreview = MakeShared(CEditor3DPreviewScene, *BaseManager, *renderWindowForModelPreview);
				renderWindowForModelPreview->SetRenderWindowEventListener(std::dynamic_pointer_cast<IRenderWindowEventListener>(sceneForPreview));
				renderWindowForModelPreview->SetNativeWindowEventListener(std::dynamic_pointer_cast<INativeWindowEventListener>(sceneForPreview));
				sceneForPreview->Initialize();

				editorScene->SetPreviewScene(sceneForPreview);
			}
		}



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
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetBreakAlloc(416);

	main_internal(argumentCount, arguments);		

	if (BaseManager)
		delete BaseManager;

	//_CrtMemDumpAllObjectsSince(NULL);

	return 0;
}
