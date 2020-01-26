#pragma once

class ZN_API CNativeWindowFactory
	: public INativeWindowFactory
{
public:
	CNativeWindowFactory(IApplication * Application);
	virtual ~CNativeWindowFactory();
	
	//
	// IWindowCreator
	//
	std::shared_ptr<INativeWindow> CreateWindowInstance(LPCWSTR WindowName, LONG Width, LONG Height) override;
	
private:
	IApplication* m_Application;
};