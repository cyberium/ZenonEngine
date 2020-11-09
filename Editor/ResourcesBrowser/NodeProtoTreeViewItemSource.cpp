#include "stdafx.h"

// General
#include "NodeProtoTreeViewItemSource.h"

CzNodeProtoTreeViewItemSource::CzNodeProtoTreeViewItemSource(const std::shared_ptr<ISceneNode>& SceneNode)
	: m_SceneNode(SceneNode)
{}

CzNodeProtoTreeViewItemSource::~CzNodeProtoTreeViewItemSource()
{

}



//
// IznTreeViewItemSource
//
ETreeViewItemType CzNodeProtoTreeViewItemSource::GetType() const
{
	return ETreeViewItemType::SceneNodeProto;
}

std::string CzNodeProtoTreeViewItemSource::GetText() const
{
	return m_SceneNode->GetName();
}

std::shared_ptr<IObject> CzNodeProtoTreeViewItemSource::GetObject_() const
{
	return m_SceneNode;
}
