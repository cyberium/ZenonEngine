#include "stdafx.h"

// General
#include "SceneBrowserNode.h"




CznSceneBrowserNode::CznSceneBrowserNode(std::shared_ptr<IObject> Object)
	: m_Object(Object)
{
}



//
// IznSceneBrowserNode
//
Guid CznSceneBrowserNode::GetId() const
{
	return m_Object->GetGUID();
}

std::string CznSceneBrowserNode::GetText() const
{
	return m_Object->GetName();
}
