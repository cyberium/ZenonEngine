#pragma once

#include "WindowHandleWrapper.h"

class ZN_API CWindowObject 
	: public IWindowCreator
{
public:
	CWindowObject();
	virtual ~CWindowObject();

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
	CWindowHandleWrapper* m_WindowHandleWrapper;
};