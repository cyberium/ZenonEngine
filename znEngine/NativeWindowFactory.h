#pragma once

class ZN_API CNativeWindowFactory
	: public INativeWindowFactory
{
public:
	CNativeWindowFactory(IApplication_WindowsSpecific * Application);
	virtual ~CNativeWindowFactory();
	
	//
	// IWindowCreator
	//
	std::unique_ptr<INativeWindow> CreateWindowInstance(LPCWSTR WindowName, LONG Width, LONG Height) override;
	
private:
	const IApplication_WindowsSpecific* m_Application;
};