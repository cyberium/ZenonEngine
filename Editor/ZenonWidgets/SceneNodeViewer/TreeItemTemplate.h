#pragma once

#include <QList>
#include <QVector>
#include <QVariant>

class SceneNodeTreeModel;

template <typename T>
class CSceneNodeTreeItem
{
public:
	CSceneNodeTreeItem(SceneNodeTreeModel * Model)
		: m_SceneNode3D(nullptr)
		, m_Parent(nullptr)
		, m_Model(Model)
	{}

	CSceneNodeTreeItem(SceneNodeTreeModel * Model, const std::shared_ptr<T>& SceneNode3D, CSceneNodeTreeItem * Parent)
		: m_SceneNode3D(SceneNode3D)
		, m_Parent(Parent)
		, m_Model(Model)
	{
		for (const auto& ch : SceneNode3D->GetChilds())
		{
			addChild(new T(m_Model, ch, this));
		}
	}
	virtual ~CSceneNodeTreeItem();

	void addChild(CSceneNodeTreeItem * child)
	{
		m_Model->Add(item->GetSceneNode()->GetGuid(), item);
		m_Childs.push_back(item);
	}
	T* child(int row);
	{
		if (row >= m_Childs.size())
			return nullptr;

		return m_Childs.at(row);
	}
	int childCount() const
	{
		return m_Childs.size();
	}
	T* parentItem()
	{
		return m_Parent;
	}

	QVariant data() const
	{
		if (m_SceneNode3D == nullptr)
			return QVariant("Real root element");

		return QVariant(m_SceneNode3D->GetName().c_str());
	}
	int childNumberInParent() const
	{
		if (m_Parent)
		{
			const auto& it = std::find(m_Parent->m_Childs.begin(), m_Parent->m_Childs.end(), this);
			if (it != m_Parent->m_Childs.end())
				return std::distance(m_Parent->m_Childs.begin(), it);
		}

		return 0;
	}

	std::shared_ptr<T> GetSceneNode() const
	{
		return m_SceneNode3D;
	}

private:
	T*								m_Parent;
	std::vector<T*>					m_Childs;

	std::shared_ptr<T>	            m_SceneNode3D;
	SceneNodeTreeModel *            m_Model;
};
