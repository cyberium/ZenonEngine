#include "stdafx.h"

// Additional
#include "Scene_Default.h"


#include "BugTrap/BugTrap.h"


static IBaseManager* BaseManager = nullptr;


void main_internal(int argumentCount, char* arguments[])
{
	// 1. Initialize engine and some improtant managers
	BaseManager = InitializeEngine(Utils::ArgumentsToVector(argumentCount, arguments), "");
	
	//try
	//{
		Application app(*BaseManager, ::GetModuleHandle(NULL));
		CNativeWindowFactory nativeWindowFactory(&app);

		ULONG windowWidth = BaseManager->GetManager<ISettings>()->GetGroup("Video")->GetSettingT<glm::vec2>("WindowSize")->Get().x;
		ULONG windowHeight = BaseManager->GetManager<ISettings>()->GetGroup("Video")->GetSettingT<glm::vec2>("WindowSize")->Get().y;

		IRenderDevice& renderDevice = app.CreateRenderDevice(RenderDeviceType::RenderDeviceType_DirectX11);

		BaseManager->AddManager<IznFontsManager>(MakeShared(FontsManager, renderDevice, *BaseManager));

		{
			BaseManager->GetManager<ILoader>()->Start();

			{
				std::unique_ptr<IznNativeWindow> nativeWindow = nativeWindowFactory.CreateWindowInstance(L"Zenon Engine", windowWidth, windowHeight);

				const auto& renderWindow = renderDevice.GetObjectsFactory().CreateRenderWindow(std::move(nativeWindow), false);
				app.AddRenderWindow(renderWindow);

				std::shared_ptr<IScene> scene = MakeShared(CSceneDefault, *BaseManager, *renderWindow);
				renderWindow->SetRenderWindowEventListener(std::dynamic_pointer_cast<IRenderWindowEventListener>(scene));
				renderWindow->SetNativeWindowEventListener(std::dynamic_pointer_cast<IznNativeWindowEventListener>(scene));
				scene->Initialize();
			}

			/*{
				std::unique_ptr<IznNativeWindow> nativeWindow2 = nativeWindowFactory.CreateWindowInstance(L"Zenon Engine", windowWidth, windowHeight);

				const auto& renderWindow2 = renderDevice.GetObjectsFactory().CreateRenderWindow(std::move(nativeWindow2), false);
				app.AddRenderWindow(renderWindow2);

				std::shared_ptr<IScene> scene2 = MakeShared(CSceneDefault, *BaseManager, *renderWindow2);
				renderWindow2->SetRenderWindowEventListener(std::dynamic_pointer_cast<IRenderWindowEventListener>(scene2));
				renderWindow2->SetNativeWindowEventListener(std::dynamic_pointer_cast<IznNativeWindowEventListener>(scene2));
				scene2->Initialize();
			}*/
		}

		app.Run();

	//}
	//catch (const CznRenderException& e)
	//{
	//	Log::Fatal("RenderError: %s", e.MessageCStr());
	//}
	//catch (const CException& e)
	//{
	//	Log::Fatal("EngienError: %s", e.MessageCStr());
	//}
}


int main(int argumentCount, char* arguments[])
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

	BT_InstallSehFilter();
	BT_SetAppName(L"ZenonEngine");
	//BT_SetSupportEMail(L"your@email.com");
	BT_SetFlags(BTF_DETAILEDMODE /*| BTF_EDITMAIL*/);
	//BT_SetSupportServer(L"localhost", 9999);
	//BT_SetSupportURL(L"http://www.your-web-site.com");

	main_internal(argumentCount, arguments);		

	if (BaseManager)
		delete BaseManager;

	return 0;
}
