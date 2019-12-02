#pragma once

LRESULT CALLBACK WndProcLink(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

class OW_ENGINE_API CWindowClassRegistratorObject :
	public IWindowClassRegistrator
{
public:
	CWindowClassRegistratorObject(IApplication * Application);
	virtual ~CWindowClassRegistratorObject();

	// IWindowClassRegistrator
	void RegisterWindowClass(LPCWSTR WindowClassName) override;
	void UnregisterWindowClass() override;
	LPCWSTR GetWindowClassName() const;

private:
	HINSTANCE m_HInstance;
	std::wstring m_ClassName;
};