#include "stdafx.h"

// General
#include "znRenderDX11.h"

// Additional
#include "RenderDeviceDX11.h"

extern CLog* gLogInstance;

//
// CznRenderDX11DeviceCreator
//
class CznRenderDX11DeviceCreator : public IznRenderDeviceCreator
{
public:
	CznRenderDX11DeviceCreator(IBaseManager* BaseManager)
		: m_BaseManager(BaseManager)
	{}
	virtual ~CznRenderDX11DeviceCreator()
	{}

	// IznRenderDeviceCreator
	RenderDeviceType GetRenderDeviceType() const
	{
		return RenderDeviceType::RenderDeviceType_DirectX;
	}
	std::shared_ptr<IRenderDevice> CreateRenderDevice()
	{
		if (m_CachedRenderDevice == nullptr)
		{
			m_CachedRenderDevice = std::make_shared<RenderDeviceDX11>(m_BaseManager);
			m_CachedRenderDevice->Initialize();
		}

		return m_CachedRenderDevice;
	}

private:
	IBaseManager* m_BaseManager;
	std::shared_ptr<IRenderDevice> m_CachedRenderDevice;
};



//
// CznRenderDX11Plugin
//
class CznRenderDX11Plugin : public IznPlugin
{
public:
	CznRenderDX11Plugin()
	{}
	virtual ~CznRenderDX11Plugin()
	{}

	// IznPlugin
	bool Initialize(IBaseManager* BaseManager)
	{
		m_BaseManager = BaseManager;
		
		gLogInstance = std::dynamic_pointer_cast<CLog>(GetManager<ILog>(m_BaseManager)).get();
		m_RenderDeviceCreator = std::make_shared<CznRenderDX11DeviceCreator>(m_BaseManager);

		GetManager<IznRenderDeviceCreatorFactory>(m_BaseManager)->RegisterRenderDeviceCreator(m_RenderDeviceCreator);

		return true;
	}
	void Finalize()
	{
		GetManager<IznRenderDeviceCreatorFactory>(m_BaseManager)->UnregisterRenderDeviceCreator(m_RenderDeviceCreator);
	}

private:
	IBaseManager* m_BaseManager;
	std::shared_ptr<IznRenderDeviceCreator> m_RenderDeviceCreator;
};



IznPlugin* plugin = nullptr;
IznPlugin* WINAPI GetPlugin(IBaseManager* BaseManager)
{
	if (plugin == nullptr)
	{
		plugin = new CznRenderDX11Plugin();
	}

	return plugin;
}