#pragma once

#include "EditorInterfaces.h"
#include "TreeViewIntfs.h"

class CznTreeViewItem
	: public IznTreeViewItem
	, public IznTreeViewItemInternal
	, public std::enable_shared_from_this<CznTreeViewItem>
{
public:
	CznTreeViewItem(const IznTreeViewItem * Parent = nullptr);
	virtual ~CznTreeViewItem();

	// IznTreeViewItem
	ETreeViewItemType                               GetType() const override;
	std::string                                     GetText() const override;
	std::string                                     GetIconName() const override;
	size_t                                          GetChildsCount() const override;
	std::shared_ptr<IznTreeViewItem>                GetChild(size_t Index) const override;
	const IznTreeViewItem*                          GetParent() const override;
	size_t                                          GetMyIndexInParent() const override;
	std::shared_ptr<IObject>                        GetObject_() const override;

	// IznTreeViewItemInternal
	void                                            SetParent(IznTreeViewItem * Parent) override;
	void                                            ClearCache() override;

protected:
	virtual std::shared_ptr<IznTreeViewItem>        CreateChild(std::shared_ptr<IObject> Object) const;
	virtual std::shared_ptr<IznTreeViewItem>        GetChildInternal(std::shared_ptr<IObject> Child) const;

private:
	const IznTreeViewItem*                                   m_Parent;
	mutable std::map<Guid, std::shared_ptr<IznTreeViewItem>> m_CachedChildMap;
};
