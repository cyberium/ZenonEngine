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
	std::unique_ptr<IznNativeWindow> CreateWindowInstance(LPCWSTR WindowName, LONG Width, LONG Height) const override;
	std::unique_ptr<IznNativeWindow> CreateWindowProxy(IznNativeWindow& SourceNativeWindow) const override;

private:
	const IApplication_WindowsSpecific* m_Application_WindowsSpecific;
};