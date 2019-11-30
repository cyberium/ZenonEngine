#include "stdafx.h"

// General
#include "znRenderOpenGL.h"

// Additional
#include "RenderDeviceOGL.h"

extern std::shared_ptr<CLog> gLogInstance;

//
// CznRenderOpenGLDeviceCreator
//
class CznRenderOpenGLDeviceCreator : public IznRenderDeviceCreator
{
public:
	CznRenderOpenGLDeviceCreator(std::shared_ptr<IBaseManager> BaseManager)
		: m_BaseManager(BaseManager)
	{}
	virtual ~CznRenderOpenGLDeviceCreator()
	{}

	// IznRenderDeviceCreator
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
	std::shared_ptr<IBaseManager> m_BaseManager;
	std::shared_ptr<IRenderDevice> m_CachedRenderDevice;
};



//
// CznRenderOpenGLPlugin
//
class CznRenderOpenGLPlugin : public IznPlugin
{
public:
	CznRenderOpenGLPlugin()
	{}
	virtual ~CznRenderOpenGLPlugin()
	{}

	// IznPlugin
	bool Initialize(std::shared_ptr<IBaseManager> BaseManager)
	{
		m_BaseManager = BaseManager;
		gLogInstance = std::dynamic_pointer_cast<CLog>(GetManager<ILog>(m_BaseManager));

		m_RenderDeviceCreator = std::make_shared<CznRenderOpenGLDeviceCreator>(m_BaseManager);

		GetManager<IznRenderDeviceCreatorFactory>(m_BaseManager)->RegisterRenderDeviceCreator(m_RenderDeviceCreator);

		return false;
	}
	void Finalize()
	{
		GetManager<IznRenderDeviceCreatorFactory>(m_BaseManager)->UnregisterRenderDeviceCreator(m_RenderDeviceCreator);
	}

private:
	std::shared_ptr<IBaseManager> m_BaseManager;
	std::shared_ptr<IznRenderDeviceCreator> m_RenderDeviceCreator;
};



IznPlugin* plugin = nullptr;
IznPlugin* GetPlugin(std::shared_ptr<IBaseManager> BaseManager)
{
	if (plugin == nullptr)
	{
		plugin = new CznRenderOpenGLPlugin();
		plugin->Initialize(BaseManager);
	}

	return plugin;
}