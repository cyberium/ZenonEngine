#include "stdafx.h"

// General
#include "ScenesFactory.h"

// Additional
#include "SceneDefaultCreator.h"

CScenesFactory::CScenesFactory(IBaseManager* BaseManager)
{
	AddSceneCreator(std::make_shared<CSceneDefaultCreator>(BaseManager));
}

CScenesFactory::~CScenesFactory()
{
}



void CScenesFactory::AddSceneCreator(std::shared_ptr<ISceneCreator> Creator)
{
	for (size_t i = 0; i < Creator->GetScenesCount(); i++)
	{
		m_Creators.push_back(Creator);
		Log::Print("CScenesFactory: SceneType '%s' registered.", Creator->GetSceneTypeName(i).c_str());
	}
}

void CScenesFactory::RemoveSceneCreator(std::shared_ptr<ISceneCreator> Creator)
{
}



//
// IScenesFactory
//
std::shared_ptr<IScene> CScenesFactory::CreateScene(std::string SceneTypeName) const
{
	for (const auto& creator : m_Creators)
	{
		for (size_t i = 0; i < creator->GetScenesCount(); i++)
		{
			if (creator->GetSceneTypeName(i) == SceneTypeName)
			{
				return creator->CreateScene(i);
			}
		}
	}

	return nullptr;
}



//
// IznPluginsEventListener
//
void CScenesFactory::OnPluginAdded(std::shared_ptr<IznPlugin> Plugin)
{
	if (std::shared_ptr<ISceneCreator> creator = std::dynamic_pointer_cast<ISceneCreator>(Plugin))
	{
		AddSceneCreator(creator);
	}
}

void CScenesFactory::OnPluginInitialized(std::shared_ptr<IznPlugin> Plugin)
{

}
