#pragma once

#include "ZenonWidgets/ZenonTreeView/TreeViewItem.h"

class CTextureTreeViewItem
	: public CznTreeViewItem
{
public:
	CTextureTreeViewItem(const std::shared_ptr<ITexture>& Texture);
	virtual ~CTextureTreeViewItem();

	// IznTreeViewItem
	ETreeViewItemType GetType() const override;
	std::string GetText() const override;
	std::string GetIconName() const override;
	std::shared_ptr<IObject> GetObject_() const override;

private:
	std::shared_ptr<ITexture> m_Texture;
};