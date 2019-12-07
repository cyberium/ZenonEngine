#include "stdafx.h"

// General
#include "znRenderOpenGL.h"

// Additional
#include "RenderDeviceOGL.h"

extern CLog* gLogInstance;

//
// CznRenderOpenGLDeviceCreator
//
class CznRenderOpenGLDeviceCreator : public IznRenderDeviceCreator
{
public:
	CznRenderOpenGLDeviceCreator(IBaseManager* BaseManager)
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
	IBaseManager* m_BaseManager;
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
	bool Initialize(IBaseManager* BaseManager)
	{
		m_BaseManager = BaseManager;
		gLogInstance = std::dynamic_pointer_cast<CLog>(GetManager<ILog>(m_BaseManager)).get();

		m_RenderDeviceCreator = std::make_shared<CznRenderOpenGLDeviceCreator>(m_BaseManager);

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
		plugin = new CznRenderOpenGLPlugin();
	}

	return plugin;
}