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
	CznRenderDX11Plugin(IBaseManager& BaseManager)
		: m_BaseManager(BaseManager)
	{}
	virtual ~CznRenderDX11Plugin()
	{
		OutputDebugStringW(L"CznRenderDX11Plugin deleted.\n");
	}



	//
	// IznPlugin
	//
	bool Initialize() override
	{
		gLogInstance = dynamic_cast<CLog*>(m_BaseManager.GetManager<ILog>());

		return true;
	}
	void Finalize() override
	{
		gLogInstance = nullptr;
	}
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
	RenderDeviceType GetRenderDeviceType() const override
	{
		return RenderDeviceType::RenderDeviceType_DirectX;
	}
	std::unique_ptr<IRenderDevice> CreateRenderDevice() const override
	{
		std::unique_ptr<IRenderDeviceDX11> renderDevice = std::make_unique<RenderDeviceDX11>(m_BaseManager);
		renderDevice->Initialize();
		return std::move(renderDevice);
	}

private:
	IBaseManager& m_BaseManager;
};



extern "C" __declspec(dllexport) IznPlugin* WINAPI GetPlugin(IBaseManager* BaseManager)
{
	return new CznRenderDX11Plugin(*BaseManager);
}