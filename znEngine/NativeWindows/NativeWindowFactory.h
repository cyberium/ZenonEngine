#pragma once

class ZN_API CNativeWindowFactory
	: public IznNativeWindowFactory
{
public:
	CNativeWindowFactory(IApplication_WindowsSpecific * Application_WindowsSpecific);
	virtual ~CNativeWindowFactory();
	
	//
	// IWindowCreator
	//
	std::unique_ptr<IznNativeWindow> CreateWindowInstance(const std::string& WindowName, glm::ivec2 WindowSize) const override;
	std::unique_ptr<IznNativeWindow> CreateWindowProxy(IznNativeWindow& SourceNativeWindow) const override;

private:
	const IApplication_WindowsSpecific* m_Application_WindowsSpecific;
};