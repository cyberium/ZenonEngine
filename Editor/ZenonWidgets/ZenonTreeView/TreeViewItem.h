#pragma once

#include "TreeViewIntfs.h"

#include <QVariant>

class CznTreeViewItem
	: public std::enable_shared_from_this<CznTreeViewItem>
{
public:
	CznTreeViewItem(const std::shared_ptr<IznTreeViewItemSource>& TObject, CznTreeViewItem * Parent);
	virtual ~CznTreeViewItem();

	//void SetRoot(const std::shared_ptr<IznTreeViewItemSource>& TObject);
	//void addChild(std::shared_ptr<CznTreeViewItem> child);

	std::shared_ptr<CznTreeViewItem> child(size_t row);
	size_t childCount() const;
	CznTreeViewItem* parentItem();
	QVariant data() const;
	int childNumberInParent() const;

	std::shared_ptr<IznTreeViewItemSource> GetSourceObject() const;
	std::shared_ptr<IObject> GetTObject() const;

private:
	CznTreeViewItem*                               m_Parent;
	std::vector<std::shared_ptr<CznTreeViewItem>>  m_CachedChilds;
	std::shared_ptr<IznTreeViewItemSource>         m_SourceObject;
};
