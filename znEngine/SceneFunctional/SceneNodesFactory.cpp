#include "stdafx.h"

// General
#include "SceneNodesFactory.h"

// Additional
#include "SceneNodeDefaultCreator.h"

CSceneNodesFactory::CSceneNodesFactory(IBaseManager& BaseManager)
{
	AddSceneNodeCreator(std::make_shared<CSceneNodeDefaultCreator>(BaseManager));
}

CSceneNodesFactory::~CSceneNodesFactory()
{
}



void CSceneNodesFactory::AddSceneNodeCreator(std::shared_ptr<ISceneNodeCreator> Creator)
{
	for (size_t i = 0; i < Creator->GetSceneNodesCount(); i++)
	{
		m_Creators.push_back(Creator);
		Log::Print("CSceneNodesFactory: SceneNodeType '%s' registered.", Creator->GetSceneNodeTypeName(i).c_str());
	}
}

void CSceneNodesFactory::RemoveSceneNodeCreator(std::shared_ptr<ISceneNodeCreator> Creator)
{
}



//
// ISceneNodesFactory
//
std::shared_ptr<ISceneNode3D> CSceneNodesFactory::CreateSceneNode3D(ISceneNode3D* Parent, std::string SceneNodeTypeName) const
{
	for (const auto& creator : m_Creators)
	{
		for (size_t i = 0; i < creator->GetSceneNodesCount(); i++)
		{
			if (creator->GetSceneNodeTypeName(i) == SceneNodeTypeName)
			{
				return creator->CreateSceneNode3D(Parent, i);
			}
		}
	}

	return nullptr;
}

ISceneNodeUI* CSceneNodesFactory::CreateSceneNodeUI(ISceneNodeUI* Parent, std::string SceneNodeTypeName) const
{
	for (const auto& creator : m_Creators)
	{
		for (size_t i = 0; i < creator->GetSceneNodesCount(); i++)
		{
			if (creator->GetSceneNodeTypeName(i) == SceneNodeTypeName)
			{
				return creator->CreateSceneNodeUI(Parent, i);
			}
		}
	}

	return nullptr;
}



//
// IznPluginsEventListener
//
void CSceneNodesFactory::OnPluginAdded(std::shared_ptr<IznPlugin> Plugin)
{
	if (std::shared_ptr<ISceneNodeCreator> creator = std::dynamic_pointer_cast<ISceneNodeCreator>(Plugin))
	{
		AddSceneNodeCreator(creator);
	}
}

void CSceneNodesFactory::OnPluginInitialized(std::shared_ptr<IznPlugin> Plugin)
{

}

void CSceneNodesFactory::OnPluginFinalized(std::shared_ptr<IznPlugin> Plugin)
{
}
