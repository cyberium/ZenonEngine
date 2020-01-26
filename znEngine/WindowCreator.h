#pragma once

#include "WindowHandleWrapper.h"

class ZN_API CWindowCreator 
	: public IWindowCreator
{
public:
	CWindowCreator(IApplication * Application, LPCWSTR WindowName, LONG Width, LONG Height);
	virtual ~CWindowCreator();

	const CWindowHandleWrapper* GetHandleWrapper() const;
	
	//
	// IWindowCreator
	//
	HWND CreateWindowInstance(IApplication * Application, LPCWSTR WindowName, LONG Width, LONG Height) override;
	void DestroyWindowInstance() override;
	

private:
	HINSTANCE m_HInstance;
	std::wstring m_WindowClassName;
	HWND m_HWnd;
	std::unique_ptr<CWindowHandleWrapper> m_WindowHandleWrapper;
};