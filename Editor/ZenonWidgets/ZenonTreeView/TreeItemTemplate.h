#pragma once

#include "EditorInterfaces.h"

#include <QList>
#include <QVector>
#include <QVariant>

class CQtToZenonTreeItem
	: public std::enable_shared_from_this<CQtToZenonTreeItem>
{
public:
	CQtToZenonTreeItem()
		: m_TObject(nullptr)
		, m_Parent(nullptr)
	{}

	CQtToZenonTreeItem(const std::shared_ptr<IModelCollectionItem>& TObject, CQtToZenonTreeItem * Parent)
		: m_TObject(TObject)
		, m_Parent(Parent)
	{
		for (const auto& ch : m_TObject->GetChilds())
			addChild(MakeShared(CQtToZenonTreeItem, ch, this));
	}
	virtual ~CQtToZenonTreeItem()
	{

	}

	void SetRoot(const std::shared_ptr<IModelCollectionItem>& TObject)
	{
		m_TObject = TObject;
		for (const auto& ch : m_TObject->GetChilds())
			addChild(MakeShared(CQtToZenonTreeItem, ch, this));
	}

	void addChild(std::shared_ptr<CQtToZenonTreeItem> child)
	{
		m_Childs.push_back(child);
	}

	std::shared_ptr<CQtToZenonTreeItem> child(int row)
	{
		if (row < 0 || row >= m_Childs.size())
			return nullptr;
		return m_Childs.at(row);
	}

	int childCount() const
	{
		return static_cast<int>(m_Childs.size());
	}

	CQtToZenonTreeItem* parentItem()
	{
		return m_Parent;
	}

	QVariant data() const
	{
		if (m_TObject == nullptr)
			return QVariant("Root");
		return QVariant(m_TObject->GetName().c_str());
	}

	int childNumberInParent() const
	{
		if (m_Parent)
		{
			const auto& it = std::find(m_Parent->m_Childs.begin(), m_Parent->m_Childs.end(), shared_from_this());
			if (it != m_Parent->m_Childs.end())
				return std::distance(m_Parent->m_Childs.begin(), it);
		}

		throw CException("Child not found in parent.");
	}

	std::shared_ptr<IObject> GetTObject() const
	{
		return m_TObject->Object();
	}

private:
	CQtToZenonTreeItem*                               m_Parent;
	std::vector<std::shared_ptr<CQtToZenonTreeItem>>  m_Childs;
	std::shared_ptr<IModelCollectionItem>             m_TObject;
};
