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

std::string CzNodeProtoTreeViewItemSource::GetName() const
{
	return m_SceneNode->GetName();
}

size_t CzNodeProtoTreeViewItemSource::GetChildsCount() const
{
	return 0;
}

std::shared_ptr<IznTreeViewItemSource> CzNodeProtoTreeViewItemSource::GetChild(size_t Index) const
{
	_ASSERT(false);
	return nullptr;
}

bool CzNodeProtoTreeViewItemSource::IsEqual(const std::shared_ptr<IznTreeViewItemSource>& Other) const
{
	return false;
}

std::shared_ptr<IObject> CzNodeProtoTreeViewItemSource::Object() const
{
	return m_SceneNode;
}