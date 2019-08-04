#include "stdafx.h"

// General
#include "MainForm.h"

[System::STAThreadAttribute]
void Main(array<System::String^>^ args) 
{
	_BaseManager = std::make_shared<CBaseManager>();

	std::shared_ptr<CSettings> settings = std::make_shared<CSettings>();
	AddManager<ISettings>(settings);
	settings->AddDefaults();


	std::shared_ptr<CLog> log = std::make_shared<CLog>();
	AddManager<ILog>(log);

	std::shared_ptr<CConsole> console = std::make_shared<CConsole>();
	AddManager<IConsole>(console);
	console->AddCommonCommands();

	std::shared_ptr<IFilesManager> filesManager = std::make_shared<CFilesManager>();
	AddManager<IFilesManager>(filesManager);

	std::shared_ptr<IFilesStorage> localFilesGamedata = std::make_shared<CLocalFilesStorage>("D:\\_programming\\OpenWoW\\_gamedata\\");
	filesManager->RegisterFilesStorage(localFilesGamedata);

	HMODULE hModule = GetModuleHandle(NULL);
	std::shared_ptr<IFilesStorage> libraryFileStorage = std::make_shared<CLibraryResourceFileStotage>(hModule);
	filesManager->RegisterFilesStorage(libraryFileStorage);

	HMODULE m_HINSTANCE = ::GetModuleHandle(NULL);

	System::Windows::Forms::Application::EnableVisualStyles();
	System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false);
	znEditor::MainForm form;
	System::Windows::Forms::Application::Run(%form);
}