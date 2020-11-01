#pragma once

#include "TreeViewIntfs.h"

#include <QVariant>

class CznTreeViewItem
	: public std::enable_shared_from_this<CznTreeViewItem>
{
public:
	CznTreeViewItem(const std::shared_ptr<IznTreeViewItemSource>& TObject, CznTreeViewItem * Parent);
	virtual ~CznTreeViewItem();

	void ClearCache();

	std::shared_ptr<CznTreeViewItem>  GetChildByIndex(size_t row);
	size_t                            GetChildsCount() const;
	CznTreeViewItem*                  GetParent();
	QVariant                          GetData() const;
	size_t                            GetMyIndexInParent() const;

	std::shared_ptr<IznTreeViewItemSource> GetSourceObject() const;

private:
	std::shared_ptr<CznTreeViewItem> GetChildInternal(const std::shared_ptr<IznTreeViewItemSource>& SourceItem);

private:
	CznTreeViewItem*                               m_Parent;
	std::shared_ptr<IznTreeViewItemSource>         m_SourceObject;


	std::vector<std::shared_ptr<CznTreeViewItem>>  m_CachedChilds;

	//std::map<std::string, std::shared_ptr<CznTreeViewItem>>  m_CachedChilds;
};
