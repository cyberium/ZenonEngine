#pragma once

#include "ZenonWidgets/ZenonTreeView/TreeViewItem.h"

class CznModel3DTreeViewItemSource
	: public CznTreeViewItem
	, public CLoadableObject
{
public:
	CznModel3DTreeViewItemSource(IBaseManager& BaseManager, std::string FileName);
	virtual ~CznModel3DTreeViewItemSource();

	// IznTreeViewItem
	ETreeViewItemType GetType() const override;
	std::string GetText() const override;
	std::shared_ptr<IObject> GetObject_() const override;

	// ILoadableObject
	bool Load() override;
	bool Delete() override;

protected:
	IBaseManager& GetBaseManager() const;

private:
	std::string m_FileName;
	std::shared_ptr<IModel> m_Model;
	IBaseManager& m_BaseManager;
};