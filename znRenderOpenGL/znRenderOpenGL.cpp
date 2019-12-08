#include "stdafx.h"

// General
#include "znRenderOpenGL.h"

// Additional
#include "RenderDeviceOGL.h"

extern CLog* gLogInstance;

class CznRenderOpenGLPlugin 
	: public IznPlugin
	, public IznRenderDeviceCreator
{
public:
	CznRenderOpenGLPlugin(IBaseManager* BaseManager)
		: m_BaseManager(BaseManager)
	{}
	virtual ~CznRenderOpenGLPlugin()
	{}



	//
	// IznPlugin
	//
	bool Initialize()
	{
		gLogInstance = std::dynamic_pointer_cast<CLog>(GetManager<ILog>(m_BaseManager)).get();

		return true;
	}
	void Finalize()
	{}
	std::string GetName() const override
	{
		return "OpenGL render support plugin.";
	}
	std::string GetDescription() const override
	{
		return "Provides OpenGL 4.4 render device support.";
	}


	//
	// IznRenderDeviceCreator
	//
	RenderDeviceType GetRenderDeviceType() const
	{
		return RenderDeviceType::RenderDeviceType_OpenGL;
	}
	std::shared_ptr<IRenderDevice> CreateRenderDevice()
	{
		if (m_CachedRenderDevice == nullptr)
		{
			m_CachedRenderDevice = std::make_shared<RenderDeviceOGL>(m_BaseManager);
			m_CachedRenderDevice->Initialize();
		}

		return m_CachedRenderDevice;
	}

private:
	IBaseManager* m_BaseManager;
	std::shared_ptr<IRenderDevice> m_CachedRenderDevice;
};



IznPlugin* plugin = nullptr;
IznPlugin* WINAPI GetPlugin(IBaseManager* BaseManager)
{
	if (plugin == nullptr)
	{
		plugin = new CznRenderOpenGLPlugin(BaseManager);
	}

	return plugin;
}