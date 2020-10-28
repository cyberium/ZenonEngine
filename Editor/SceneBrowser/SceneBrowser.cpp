#include "stdafx.h"

// General
#include "SceneBrowser.h"





//
// IznSceneBrowser
//
std::shared_ptr<IznSceneBrowserNode> CSceneBrowser::GetRoot() const
{
	return std::shared_ptr<IznSceneBrowserNode>();
}

std::shared_ptr<IznSceneBrowserNode> CSceneBrowser::GetNodeById(Guid Guid) const
{
	return std::shared_ptr<IznSceneBrowserNode>();
}

void CSceneBrowser::RefreshAll()
{

}

void CSceneBrowser::RefreshNode(Guid Guid)
{

}

void CSceneBrowser::AddBuilder(const std::shared_ptr<IznSceneBrowserBuilder>& Builder)
{

}
