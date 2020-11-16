#include "stdafx.h"

// General
#include "znPluginFBXModels.h"

// Additional
#include "FBXModelsLoader.h"

CznPluginFBXModels::CznPluginFBXModels(IBaseManager & BaseManager)
	: m_BaseManager(BaseManager)
{
}

CznPluginFBXModels::~CznPluginFBXModels()
{
}



//
// IznPlugin
//
bool CznPluginFBXModels::Initialize()
{
	m_FBXModelsLoader = MakeShared(CznFBXModelsLoader, m_BaseManager);
	m_BaseManager.GetManager<IznModelsFactory>()->AddModelsLoader(m_FBXModelsLoader);

	return true;
}

void CznPluginFBXModels::Finalize()
{
	m_BaseManager.GetManager<IznModelsFactory>()->RemoveModelsLoader(m_FBXModelsLoader);
}

std::string CznPluginFBXModels::GetName() const
{
	return "Autodesk *.fbx plugin.";
}

std::string CznPluginFBXModels::GetDescription() const
{
	return "TODO: Description.";
}



//
//
//
IznPlugin* GetPlugin(IBaseManager* BaseManager)
{
	return new CznPluginFBXModels(*BaseManager);
}
