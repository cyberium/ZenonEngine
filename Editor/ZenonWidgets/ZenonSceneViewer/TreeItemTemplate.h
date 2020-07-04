#pragma once

#include <QList>
#include <QVector>
#include <QVariant>


template <typename T>
class CQtToZenonTreeModel;

template <typename T>
class CQtToZenonTreeItem

	
{
public:
	CQtToZenonTreeItem(CQtToZenonTreeModel<T>& Model)
		: m_TObject(nullptr)
		, m_Parent(nullptr)
		, m_Model(Model)
	{}

	CQtToZenonTreeItem(CQtToZenonTreeModel<T>& Model, const std::shared_ptr<T>& TObject, CQtToZenonTreeItem * Parent)
		: m_TObject(TObject)
		, m_Parent(Parent)
		, m_Model(Model)
	{
		for (const auto& ch : m_TObject->GetChilds())
		{
			addChild(new CQtToZenonTreeItem<T>(m_Model, ch, this));
		}
	}
	virtual ~CQtToZenonTreeItem()
	{

	}

	void SetRoot(const std::shared_ptr<T>& TObject)
	{
		m_TObject = TObject;
		for (const auto& ch : m_TObject->GetChilds())
			addChild(new CQtToZenonTreeItem<T>(m_Model, ch, this));
	}

	void addChild(CQtToZenonTreeItem<T> * child)
	{
		m_Model.Add(child->GetTObject()->GetGuid(), child);
		m_Childs.push_back(child);
	}

	CQtToZenonTreeItem<T>* child(int row)
	{
		if (row < 0 || row >= m_Childs.size())
			return nullptr;
		return m_Childs.at(row);
	}
	int childCount() const
	{
		return m_Childs.size();
	}
	CQtToZenonTreeItem<T>* parentItem()
	{
		return m_Parent;
	}

	QVariant data() const
	{
		if (m_TObject == nullptr)
			return QVariant("Root.");
		return QVariant(m_TObject->GetName().c_str());
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

	std::shared_ptr<T> GetTObject() const
	{
		return m_TObject;
	}

private:
	CQtToZenonTreeItem<T>*			    m_Parent;
	std::vector<CQtToZenonTreeItem<T>*>	m_Childs;

	std::shared_ptr<T>	                m_TObject;
	CQtToZenonTreeModel<T>&             m_Model;
};
