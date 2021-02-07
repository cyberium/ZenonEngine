#include "stdafx.h"

// Genreal
#include "ApplicationNative.h"

// Additional
#include "znEngine.h"

CApplicationNative::CApplicationNative()
{
}


CApplicationNative::~CApplicationNative()
{

}





IRenderDevice& CApplicationNative::CreateRenderDevice(RenderDeviceType DeviceType)
{
	m_RenderDevice = GetBaseManager().GetManager<IznRenderDeviceFactory>()->GetRenderDeviceCreator(DeviceType).CreateRenderDevice();
	return GetRenderDevice();
}

void CApplicationNative::AddRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow)
{
	const auto& it = std::find(m_Windows.begin(), m_Windows.end(), RenderWindow);
	if (it != m_Windows.end())
		throw CException("CApplicationNative: RenderWindow already exists.");
	m_Windows.push_back(RenderWindow);
}

void CApplicationNative::RemoveRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow)
{
	const auto& it = std::find(m_Windows.begin(), m_Windows.end(), RenderWindow);
	if (it == m_Windows.end())
		throw CException("CApplicationNative: Unable find RenderWindow.");
	m_Windows.erase(it);
}



//
// IApplication
//

IRenderDevice& CApplicationNative::GetRenderDevice() const
{
	_ASSERT(m_RenderDevice != nullptr);
	return *m_RenderDevice;
}



//
// Protected
//
void CApplicationNative::DoBeforeRunImpl()
{
	for (const auto& w : m_Windows)
		if (auto applicationEventsListener = std::dynamic_pointer_cast<IApplicationEventsListener>(w))
			applicationEventsListener->OnInitialize(EventArgs());
}

void CApplicationNative::DoRunImpl(UpdateEventArgs& UpdateEventArgs)
{
	for (const auto& w : m_Windows)
		if (auto applicationEventsListener = std::dynamic_pointer_cast<IApplicationEventsListener>(w))
			applicationEventsListener->OnUpdate(UpdateEventArgs);
}

void CApplicationNative::DoAfterRunImpl()
{
	for (const auto& w : m_Windows)
		if (auto applicationEventsListener = std::dynamic_pointer_cast<IApplicationEventsListener>(w))
			applicationEventsListener->OnExit(EventArgs());
	m_Windows.clear();
}

void CApplicationNative::OnStopAfterPluginManager()
{
	m_RenderDevice.reset();
}
