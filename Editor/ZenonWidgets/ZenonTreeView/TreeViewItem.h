#pragma once

#include "TreeViewIntfs.h"

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
	std::string                       GetText() const;
	size_t                            GetMyIndexInParent() const;

	std::shared_ptr<IznTreeViewItemSource> GetSourceObject() const;

private:
	std::shared_ptr<CznTreeViewItem> GetChildInternal(const std::shared_ptr<IznTreeViewItemSource>& SourceItem);

private:
	CznTreeViewItem*                                         m_Parent;
	std::shared_ptr<IznTreeViewItemSource>                   m_SourceObject;

	mutable std::map<Guid, std::shared_ptr<CznTreeViewItem>> m_CachedChildMap;
};
