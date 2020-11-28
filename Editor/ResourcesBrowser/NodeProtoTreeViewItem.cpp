#include "stdafx.h"

// General
#include "NodeProtoTreeViewItem.h"

CNodeProtoTreeViewItem::CNodeProtoTreeViewItem(const std::shared_ptr<ISceneNode>& SceneNode)
	: m_SceneNode(SceneNode)
{}

CNodeProtoTreeViewItem::~CNodeProtoTreeViewItem()
{

}



//
// IznTreeViewItem
//
ETreeViewItemType CNodeProtoTreeViewItem::GetType() const
{
	return ETreeViewItemType::SceneNodeProto;
}

std::string CNodeProtoTreeViewItem::GetText() const
{
	return m_SceneNode->GetName();
}

std::shared_ptr<IObject> CNodeProtoTreeViewItem::GetObject_() const
{
	return m_SceneNode;
}
