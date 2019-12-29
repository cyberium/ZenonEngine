#include "stdafx.h"

// General
#include "SceneNodeDefaultCreator.h"

// Additional
#include "UI/UIText.h"
#include "Models/FBX/FBXSceneNode.h"

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
		return "SceneNode3D";
	}
	else if (Index == 1)
	{
		return "SceneNodeUI";
	}
	else if (Index == 2)
	{
		return "TextUI";
	}
	else if (Index == 3)
	{
		return "FBXSceneNode";
	}

	throw CException("CSceneNodeDefaultCreator: GetSceneNodeTypeName(%d) is out of bounds. Count = %d", Index, GetSceneNodesCount());
}

std::shared_ptr<ISceneNode> CSceneNodeDefaultCreator::CreateSceneNode(std::weak_ptr<ISceneNode> Parent, size_t Index) const
{
	if (Index == 0)
	{
		return Parent.lock()->CreateSceneNode<SceneNode3D>();
	}
	else if (Index == 1)
	{
		return Parent.lock()->CreateSceneNode<CUIBaseNode>();
	}
	else if (Index == 2)
	{
		return Parent.lock()->CreateSceneNode<CUITextNode>();
	}
	else if (Index == 3)
	{
		return Parent.lock()->CreateSceneNode<CFBXSceneNode>("Sponza\\Sponza.fbx");
		//return Parent.lock()->CreateSceneNode<CFBXSceneNode>("crytek-sponza\\sponza_nobanner.obj");
	}

	throw CException("CSceneNodeDefaultCreator: CreateSceneNode(%d) is out of bounds. Count = %d", Index, GetSceneNodesCount());
}
