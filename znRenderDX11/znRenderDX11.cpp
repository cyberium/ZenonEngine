#include "stdafx.h"

// General
#include "znRenderDX11.h"

// Additional
#include "RenderDeviceDX11.h"

extern CLog* gLogInstance;

class CznRenderDX11Plugin 
	: public IznPlugin
	, public IznRenderDeviceCreator
{
public:
	CznRenderDX11Plugin(IBaseManager* BaseManager)
		: m_BaseManager(BaseManager)
	{}
	virtual ~CznRenderDX11Plugin()
	{}



	//
	// IznPlugin
	//
	bool Initialize() override
	{
		gLogInstance = std::dynamic_pointer_cast<CLog>(m_BaseManager->GetManager<ILog>()).get();

		return true;
	}
	void Finalize() override
	{}
	std::string GetName() const override
	{
		return "DirectX render support plugin.";
	}
	std::string GetDescription() const override
	{
		return "Provides DirectX 11 render device support.";
	}

	//
	// IznRenderDeviceCreator
	//
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



IznPlugin* plugin = nullptr;
extern "C" __declspec(dllexport) IznPlugin* WINAPI GetPlugin(IBaseManager* BaseManager)
{
	if (plugin == nullptr)
	{
		plugin = new CznRenderDX11Plugin(BaseManager);
	}

	return plugin;
}