#include "stdafx.h"

IBaseManager* BaseManager = nullptr;

void main_internal(int argumentCount, char* arguments[])
{
	// 1. Initialize engine and some improtant managers
	BaseManager = InitializeEngine(ArgumentsToVector(argumentCount, arguments), "");

	// 3. Create application
	Application app(BaseManager, ::GetModuleHandle(NULL));


	//CXMLManager xmlM;

	// Reader
	//std::shared_ptr<IXMLReader> reader = xmlM.CreateReader(BaseManager->GetManager<IFilesManager>()->Open("Scene.xml"));



	CNativeWindowFactory nativeWindowFactory(&app);

	std::shared_ptr<INativeWindow> nativeWindow = nativeWindowFactory.CreateWindowInstance(
		L"Zenon Engine",
		BaseManager->GetManager<ISettings>()->GetGroup("Video")->GetSettingT<glm::vec2>("WindowSize")->Get().x,
		BaseManager->GetManager<ISettings>()->GetGroup("Video")->GetSettingT<glm::vec2>("WindowSize")->Get().y
	);

	IRenderDevice* renderDevice = app.CreateRenderDevice(RenderDeviceType::RenderDeviceType_DirectX);
	BaseManager->AddManager<IRenderDevice>(std::shared_ptr<IRenderDevice>(renderDevice));

	std::shared_ptr<IFontsManager> fontsManager = std::make_shared<FontsManager>(*renderDevice, BaseManager);
	BaseManager->AddManager<IFontsManager>(fontsManager);

	std::shared_ptr<IRenderWindow> firstRenderWindow = renderDevice->GetObjectsFactory().CreateRenderWindow(*nativeWindow, false);
	app.AddRenderWindow(firstRenderWindow);

	std::shared_ptr<IScene> scene = BaseManager->GetManager<IScenesFactory>()->CreateScene("SceneDefault");
	scene->ConnectEvents(std::dynamic_pointer_cast<IRenderWindowEvents>(firstRenderWindow));
	scene->Initialize();

	app.Run();
}

int main(int argumentCount, char* arguments[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

	main_internal(argumentCount, arguments);		

	delete BaseManager;

	_CrtMemDumpAllObjectsSince(NULL);

	return 0;
}
