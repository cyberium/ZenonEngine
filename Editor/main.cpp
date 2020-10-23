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

		QApplication a(argc, argv);

		CEditorUIFrame editorUI(editor);
		editorUI.InitializeEditorFrame();

		std::shared_ptr<IRenderWindow> renderWindow;
		std::shared_ptr<CEditor3DFrame> editorScene;
		{
			renderWindow = renderDevice.GetObjectsFactory().CreateRenderWindow(*editorUI.getMainEditor(), false);
			app.AddRenderWindow(renderWindow);

			editorScene = MakeShared(CEditor3DFrame, editor);
			editorScene->SetRenderWindow(renderWindow);
			editorScene->ConnectEvents(std::dynamic_pointer_cast<IRenderWindowEvents>(renderWindow));
			editorScene->Initialize();
		}

		editorScene->InitializeEditorFrame();

		// Scene for preview
		std::shared_ptr<CEditor3DPreviewScene> sceneForPreview;
		std::shared_ptr<IRenderWindow> renderWindowForModelPreview;
		{
			renderWindowForModelPreview = renderDevice.GetObjectsFactory().CreateRenderWindow(*editorUI.getModelPreview(), false);
			app.AddRenderWindow(renderWindowForModelPreview);
			sceneForPreview = MakeShared(CEditor3DPreviewScene, *BaseManager);
			sceneForPreview->SetRenderWindow(renderWindowForModelPreview);
			sceneForPreview->ConnectEvents(std::dynamic_pointer_cast<IRenderWindowEvents>(renderWindowForModelPreview));
			sceneForPreview->Initialize();

			editorScene->SetPreviewScene(sceneForPreview);
		}

		editor.GetTools().Initialize();

		editorUI.showMaximized();


		std::shared_ptr<IDebugOutput> debugOutput(editorUI.getUI().LogViewer);
		BaseManager->GetManager<ILog>()->AddDebugOutput(debugOutput);
		
		QTimer *timer = new QTimer(&editorUI);
		editorUI.connect(timer, &QTimer::timeout, &editorUI, [&app] {
			app.DoRun();
		});

		app.DoBeforeRun();
		{
			timer->start();
			{
				a.exec();
			}
			timer->stop();

			a.closeAllWindows();
		}

		BaseManager->GetManager<ILog>()->DeleteDebugOutput(debugOutput);

		app.DoAfterRun();
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

	main_internal(argumentCount, arguments);		

	if (BaseManager)
	{
		BaseManager->RemoveAllManagers();
		delete BaseManager;
	}

	_CrtMemDumpAllObjectsSince(NULL);

	return 0;
}
