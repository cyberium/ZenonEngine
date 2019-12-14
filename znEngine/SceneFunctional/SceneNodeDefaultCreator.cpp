#include "stdafx.h"

// General
#include "SceneNodeDefaultCreator.h"

// Additional
#include "UI/UIText.h"

CSceneNodeDefaultCreator::CSceneNodeDefaultCreator(IBaseManager* BaseManager)
	: m_BaseManager(BaseManager)
{}

CSceneNodeDefaultCreator::~CSceneNodeDefaultCreator()
{}



//
// ISceneNodeCreator
//
size_t CSceneNodeDefaultCreator::GetSceneNodesCount() const
{
	return 4;
}

std::string CSceneNodeDefaultCreator::GetSceneNodeTypeName(size_t Index) const
{
	if (Index == 0)
	{
		return "SceneNodeBase";
	}
	else if (Index == 1)
	{
		return "SceneNode3D";
	}
	else if (Index == 2)
	{
		return "SceneNodeUI";
	}
	else if (Index == 3)
	{
		return "TextUI";
	}

	return nullptr;
}

std::shared_ptr<ISceneNode> CSceneNodeDefaultCreator::CreateSceneNode(std::weak_ptr<ISceneNode> Parent, size_t Index) const
{
	if (Index == 0)
	{
		return Parent.lock()->CreateSceneNode<SceneNodeBase>();
	}
	else if (Index == 1)
	{
		return Parent.lock()->CreateSceneNode<SceneNode3D>();
	}
	else if (Index == 2)
	{
		return Parent.lock()->CreateSceneNode<CUIBaseNode>();
	}
	else if(Index == 3)
	{
		return Parent.lock()->CreateSceneNode<CUITextNode>();
	}

	return nullptr;
}
