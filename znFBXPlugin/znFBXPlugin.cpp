#include "stdafx.h"

// Additional
#include "FBXSceneNode.h"

extern CLog* gLogInstance;

class CznFBXPlugin 
	: public IznPlugin
	, public ISceneNodeCreator
{
public:
	CznFBXPlugin(IBaseManager* BaseManager)
		: m_BaseManager(BaseManager)
	{

	}
	virtual ~CznFBXPlugin()
	{
	}



	//
	// IznPlugin
	//
	bool Initialize()
	{
		gLogInstance = std::dynamic_pointer_cast<CLog>(GetManager<ILog>(m_BaseManager)).get();

		return true;
	}
	void Finalize()
	{

	}
	std::string GetName() const override final
	{
		return "FBX plguin";
	}
	std::string GetDescription() const override final
	{
		return "";
	}



	//
	// ISceneNodeCreator
	//
	size_t GetSceneNodesCount() const override
	{
		return 1;
	}
	std::string GetSceneNodeTypeName(size_t Index) const override
	{
		if (Index == 0)
		{
			return "FBXSceneNode";
		}

		return nullptr;
	}
	std::shared_ptr<ISceneNode> CreateSceneNode(std::weak_ptr<ISceneNode> Parent, size_t Index) const override
	{
		if (Index == 0)
		{
			return Parent.lock()->CreateSceneNode<CFBXSceneNode>("D:\\_programming\\Sponza\\Sponza.fbx", nullptr);
		}

		return nullptr;
	}

private:
	IBaseManager* m_BaseManager;
};

IznPlugin* plugin = nullptr;
extern "C" __declspec(dllexport) IznPlugin* GetPlugin(IBaseManager* BaseManager)
{
	if (plugin == nullptr)
	{
		plugin = new CznFBXPlugin(BaseManager);
	}

	return plugin;
}
