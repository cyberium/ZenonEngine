#include "stdafx.h"

// Additional
#include "MainEditor.h"
#include <QApplication>

#include "Editor3DFrame.h"
#include "Editor3DPreviewScene.h"

static IBaseManager* BaseManager = nullptr;

class EvtLoopTicker 
	: public QObject
{
	//Q_OBJECT
public:

	EvtLoopTicker(QObject * parent, Application& App)
		: QObject(parent)
		, m_Application(App)
	{
		m_Application.DoBeforeRun();
	}

public slots:
	void launch()
	{
		tickNext();
	}

private slots:
	void tick()
	{
		m_Application.Run();

		// Continue ticking
		tickNext();
	}

private:
	void tickNext()
	{
		// Trigger the tick() invokation when the event loop runs next time
		QMetaObject::invokeMethod(this, "tick", Qt::QueuedConnection);
	}

	Application& m_Application;

};


void main_internal(int argc, char *argv[])
{
	// 1. Initialize engine and some improtant managers
	BaseManager = InitializeEngine(Utils::ArgumentsToVector(argc, argv), "");

	// 3. Create application
	Application app(*BaseManager, ::GetModuleHandle(NULL));

	QApplication a(argc, argv);
	MainEditor editorUI;
	editorUI.showMaximized();

	IRenderDevice& renderDevice = app.CreateRenderDevice(RenderDeviceType::RenderDeviceType_DirectX);

	std::shared_ptr<IFontsManager> fontsManager = std::make_shared<FontsManager>(renderDevice, *BaseManager);
	BaseManager->AddManager<IFontsManager>(fontsManager);

	BaseManager->GetManager<ILoader>()->Start();

	
		std::shared_ptr<IRenderWindow> renderWindowForModelPreview = renderDevice.GetObjectsFactory().CreateRenderWindow(*editorUI.getModelPreview(), false);
		app.AddRenderWindow(renderWindowForModelPreview);

		//std::shared_ptr<IScene> scene = BaseManager->GetManager<IScenesFactory>()->CreateScene("SceneDefault");
		std::shared_ptr<CEditor3DPreviewScene> sceneForPreview = std::make_shared<CEditor3DPreviewScene>(*BaseManager);
		sceneForPreview->SetRenderWindow(renderWindowForModelPreview);
		//sceneForPreview->ConnectEvents(std::dynamic_pointer_cast<IRenderWindowEvents>(renderWindowForModelPreview));
		sceneForPreview->Initialize();
	

		
			std::shared_ptr<IRenderWindow> renderWindow = renderDevice.GetObjectsFactory().CreateRenderWindow(*editorUI.getMainEditor(), false);
			app.AddRenderWindow(renderWindow);

			//std::shared_ptr<IScene> scene = BaseManager->GetManager<IScenesFactory>()->CreateScene("SceneDefault");
			std::shared_ptr<CEdtor3DFrame> scene = std::make_shared<CEdtor3DFrame>(*BaseManager);

			scene->SetEditorUI(&editorUI);
			scene->SetPreviewScene(sceneForPreview);
			editorUI.SetEditor3D(scene.get());

			scene->SetRenderWindow(renderWindow);
			scene->ConnectEvents(std::dynamic_pointer_cast<IRenderWindowEvents>(renderWindow));
			scene->Initialize();
		
	

	editorUI.show();

	app.DoBeforeRun();

	QTimer *timer = new QTimer(&editorUI);
	editorUI.connect(timer, &QTimer::timeout, &editorUI, [&app] {
		app.DoRun();
	});
	timer->start();

	std::shared_ptr<IDebugOutput> debugOutput(editorUI.getUI().LogViewer);
	BaseManager->GetManager<ILog>()->AddDebugOutput(debugOutput);

	a.exec();

	BaseManager->GetManager<ILog>()->DeleteDebugOutput(debugOutput);

	timer->stop();
	a.closeAllWindows();
	
	app.DoAfterRun();
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
