#pragma once

#include "EditorInterfaces.h"

#include <QList>
#include <QVector>
#include <QVariant>

class CQtToZenonTreeItem
	: public std::enable_shared_from_this<CQtToZenonTreeItem>
{
public:
	CQtToZenonTreeItem();
	CQtToZenonTreeItem(const std::shared_ptr<IModelCollectionItem>& TObject, std::weak_ptr<CQtToZenonTreeItem> Parent);
	virtual ~CQtToZenonTreeItem();

	void AddChild(std::shared_ptr<CQtToZenonTreeItem> child);
	size_t GetChildsCount() const;
	std::shared_ptr<CQtToZenonTreeItem> GetChild(size_t Index) const;
	size_t GetChildIndex(const std::shared_ptr<const CQtToZenonTreeItem>& Child) const;
	int GetMyIndexInParent() const;
	std::shared_ptr<CQtToZenonTreeItem> GetParent() const;

	std::string GetText() const;
	std::shared_ptr<IObject> GetTObject() const;

private:
	std::weak_ptr<CQtToZenonTreeItem>                m_Parent;
	std::vector<std::shared_ptr<CQtToZenonTreeItem>> m_Childs;
	std::shared_ptr<IModelCollectionItem>            m_TObject;
};
