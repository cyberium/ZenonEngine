#include "stdafx.h"

// General
#include "SceneNodeDefaultCreator.h"

// Additional
#include "3D/SceneNode3D.h"
#include "UI/SceneNodeUI.h"

CSceneNodeDefaultCreator::CSceneNodeDefaultCreator()
{}

CSceneNodeDefaultCreator::~CSceneNodeDefaultCreator()
{}



//
// ISceneNodeCreator
//
size_t CSceneNodeDefaultCreator::GetSceneNodesCount() const
{
	return 2;
}

std::string CSceneNodeDefaultCreator::GetSceneNodeTypeName(size_t Index) const
{
	if (Index == 0)
	{
		return "SceneNode3D";
	}
	else if (Index == 1)
	{
		return "SceneNodeUI";
	}

	throw std::exception(("CSceneNodeDefaultCreator::GetSceneNodeTypeName: Index '" + std::to_string(Index) + "' out of bounds.").c_str());
}

std::shared_ptr<ISceneNode> CSceneNodeDefaultCreator::CreateSceneNode(std::shared_ptr<ISceneNode> Parent, size_t Index) const
{
	if (Index == 0)
	{
		return Parent->CreateSceneNode<SceneNode3D>();
	}
	else if (Index == 1)
	{
		return Parent->CreateSceneNode<CUIBaseNode>();
	}

	throw std::exception(("CSceneNodeDefaultCreator::CreateSceneNode: Index '" + std::to_string(Index) + "' out of bounds.").c_str());
}
