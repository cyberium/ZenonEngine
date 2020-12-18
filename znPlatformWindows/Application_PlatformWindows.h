#pragma once

class ZN_API CApplication_PlatformWindows
	: public CApplicationBase
	, public IApplication_PlatformBase
	, public IApplication_WindowsSpecific
{
public:
	CApplication_PlatformWindows();
	virtual ~CApplication_PlatformWindows();

	// IApplication
	std::unique_ptr<IznNativeWindow>                 CreateNativeWindow(const std::string& WindowName, glm::ivec2 WindowSize) const override;
	std::unique_ptr<IznNativeWindow>                 CreateNativeWindowProxy(IznNativeWindow& SourceNativeWindow) const override;

	// IApplication_PlatformBase
	void                                             ShowMessageBox(const std::string& Title, const std::string& Message) const override;
	std::string                                      GetExecutablePath() const override;
	IznPlugin*                                       LoadPlugin(const std::string& PluginFilename) const override;
	void                                             UnloadPlugin(const std::string& PluginFilename) const override;

	// IApplication_WindowsSpecific
	HINSTANCE                                        GetHInstance() const override;

protected:
	int ProcessEvents() override;
	void ExitApplication() override;

private:
	HINSTANCE m_HInstance;
};