#include "stdafx.h"

// General
#include "SceneNodeTreeItem.h"

CSceneNodeTreeItem::CSceneNodeTreeItem()
	: m_SceneNode3D(nullptr)
	, m_Parent(nullptr)
{
}

CSceneNodeTreeItem::CSceneNodeTreeItem(std::shared_ptr<ISceneNode> SceneNode3D, CSceneNodeTreeItem* Parent)
	: m_SceneNode3D(SceneNode3D)
	, m_Parent(Parent)
{
	for (const auto& ch : SceneNode3D->GetChilds())
	{
		appendChild(new CSceneNodeTreeItem(ch, this));
	}
}

CSceneNodeTreeItem::~CSceneNodeTreeItem() 
{ 
	qDeleteAll(m_Childs); 
}


void CSceneNodeTreeItem::appendChild(CSceneNodeTreeItem* item) 
{
	m_Childs.push_back(item);
}

CSceneNodeTreeItem* CSceneNodeTreeItem::child(int row) 
{
	return m_Childs.at(row);
}

int CSceneNodeTreeItem::childCount() const 
{
	return m_Childs.size();
}

QVariant CSceneNodeTreeItem::data() const
{
	if (m_SceneNode3D == nullptr)
		return QVariant("Real root element");

	return QVariant(m_SceneNode3D->GetName().c_str());
}

CSceneNodeTreeItem* CSceneNodeTreeItem::parentItem() 
{
	return m_Parent;
}

int CSceneNodeTreeItem::childNumber() const 
{
	if (m_Parent)
	{
		const auto& elemIter = std::find(m_Parent->m_Childs.begin(), m_Parent->m_Childs.end(), this);
		if (elemIter != m_Parent->m_Childs.end())
			return std::distance(m_Parent->m_Childs.begin(), elemIter);
	}

	return 0;
}
