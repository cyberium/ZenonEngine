#pragma once

#include <QList>
#include <QVector>
#include <QVariant>

class CSceneNodeTreeItem
{
public:
	CSceneNodeTreeItem(std::shared_ptr<SceneNode> SceneNode3D, CSceneNodeTreeItem * Parent = nullptr);
	virtual ~CSceneNodeTreeItem();

	void											appendChild(CSceneNodeTreeItem *child);
	CSceneNodeTreeItem*								child(int row); 
	int												childCount() const; 
	QVariant										data() const;
	int												childNumber() const; 

	CSceneNodeTreeItem*								parentItem();

private:
	CSceneNodeTreeItem*								m_Parent;
	std::vector<CSceneNodeTreeItem*>				m_Childs;

	std::shared_ptr<SceneNode>						m_SceneNode3D;	
};
