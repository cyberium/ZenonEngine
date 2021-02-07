#pragma once

class ZN_API CApplicationConsole_PlatformWindows
	: public CApplicationConsole
	, public IApplication_PlatformBase
	, public IApplication_WindowsSpecific
{
public:
	CApplicationConsole_PlatformWindows();
	virtual ~CApplicationConsole_PlatformWindows();

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