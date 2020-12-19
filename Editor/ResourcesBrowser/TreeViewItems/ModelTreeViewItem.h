#pragma once

#include "ZenonWidgets/ZenonTreeView/TreeViewItem.h"

class CModelTreeViewItem
	: public CznTreeViewItem
	, public CLoadableObject
{
public:
	CModelTreeViewItem(IBaseManager& BaseManager, std::string ModelFileName);
	virtual ~CModelTreeViewItem();

	// IznTreeViewItem
	ETreeViewItemType GetType() const override;
	std::string GetText() const override;
	std::string GetIconName() const override;
	std::shared_ptr<IObject> GetObject_() const override;

	// ILoadableObject
	bool Load() override;
	bool Delete() override;

private:
	std::string m_ModelFileName;
	std::shared_ptr<IModel> m_Model;
	IBaseManager& m_BaseManager;
};